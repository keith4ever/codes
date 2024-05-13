import os
import torch 

def train():
    if global_rank == 0:
        initialize_services()

    data_loader = DataLoader(train_dataset, shuffle=False, 
                    sampler=DistributedSampler(train_dataset, shuffle=True))
    model = MyModel()
    if os.path.exists('latest_checkpoint.pth') # load latest checkpoint
        model.load_state_dict(torch.load('latest_checkpoint.pth'))

    model = DistributedDataParallel(model, device_ids=[local_rank])
    optimizer = torch.optim.Adam(model.parameters(), lr=10e-4, eps=1e-9)
    loss_fn = torch.nn.CrossEntropyLoss()

    for epoch in range(num_epochs):
        for data, labels in data_loader:
            if (step_number + 1) % 100 != 0 and not last_step: # accumulate gradients for 100 steps
                with model.no_sync(): # disable gradient sync
                    loss = loss_fn(model(data), labels) # forward step
                    loss.backward() # backward step + gradient accumulation
            else:
                loss = loss_fn(model(data), labels) # forward step
                loss.backward() # backward step + gradient accumulation
                optimizer.step() # update weights
                optimizer.zero_grad() # REset gradient to zero
        
        if global_rank == 0:
            collect_statistics()
    
    if global_rank == 0: # node 0
        # also save the optimizer state and other variables needed to restore the training state
        torch.save(model.state_dict(), 'latest_checkpoint.pth')

if __name__ == '__main__':
    local_rank = int(os.environ['LOCAL_RANK'])
    global_rank = int(os.environ['RANK'])

    init_process_group(backend='nccl')
    torch.cuda.set_device(local_rank) # set the device local_rank

    train()

    destroy_process_group()