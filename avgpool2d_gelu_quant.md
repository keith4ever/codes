# Hello from Argmax

This is the take-home assignment for the ML Performance Engineer role. 
We are excitedthat you are investing time in showing off your skills to Argmax!
- This is the technical part of the interview process and it is the ONLY round.
- We want to be respectful of your time so the exercise is calibrated to take roughly ~4hours
- This is equivalent to a traditional single-day back-to-back interview schedule at BigTech.
- However, you may complete this interview over the course of 5 days at yourconvenience.
- We think this is the best way to see how you approach a new problem and present your results instead of Leetcode or brain teasers.
- Please feel free to consult with any resource as you see fi t (e.g. forums, docs, papers,other repositories) or ask us clarifying questions.
- This exercise should be possible to complete on the free-tier T4 Colab instances.

## Exercise Definition

At Argmax, we care about the inference performance of neural networks. Across platformsof interest, GPUs are the most common hardware accelerator and frequently the only onewith a general purpose programming model available for compute acceleration (as opposed to NPUs with SDKs & APIs, layers of obfuscation, and hidden fallbacks that harm performance). A key skill for the performance engineering role at Argmax is general purpose GPU computing.

One common strategy for improving performance of neural networks is to fuse kernels of adjacent layers in to single kernels to be executed by the hardware - this reduces overhead between layer dispatches, reduces the number of kernel launches, and can also permit better solutions for memory reuse or elide allocations altogether.

### Problem Statement
- Step 1:
Implement a fused operation corresponding to average pool 2d (https://pytorch.org/docs/stable/generated/torch.nn.AvgPool2d.html)
fused with a GELU activation (https://pytorch.org/docs/stable/generated/torch.nn.GELU.html)
templated for float16, and float32, using Triton, OpenCL, CUDA, Metal, or any other GPU shading language of your choice.
You may assume an input tensor of size [1, C,H, W], and limit the average pool 2d implementation to consider scalar values for kernel size and stride parameters; you may assume padding = 0. Please consider scalar values of 1, 2, and 3 for support.
- Step 2:
Add support for 8-bit input mapping to float16 or float32 output, with dequantization performed on-the-fly within the kernel. 8-bit quantization may beimplemented via a simple linear quantization scheme, applied per-channel. For a representative formula, see formula 12 in
Quantization and Training of Neural Networks for Efficient Integer-Arithmetic-Only Inference (https://arxiv.org/pdf/1712.05877)
- Step 3:
Test your kernels for correctness. Provide discussion on how things may fail in production, guarding against client misuse, quantization scheme, or anything else you find interesting.
- Step 4:
[optional] Provide discussion on relevant technologies for doing this automatically or through code generation techniques.

## Results Delivery
Please deliver a testable artifact. Colab notebook or other cloud-based execution environment is preferred for portability, but anything you find most expedient for the task is suitable.