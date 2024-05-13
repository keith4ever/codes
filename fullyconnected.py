import numpy as np 

class FullyConnectedLayer:
    def __init__(self, input_size, output_size):
        self.input_size = input_size
        self.output_size = output_size 
        self.input_data = []
        self.output_data = []
        self.weights = np.random.rand(input_size, output_size)
        self.bias = np.zeros(1, output_size)

    # def forward(self, input_data):
    #     for j in [0, self.output_size]:
    #         for i in [0, self.input_size]:
    #             self.output_data[j] += input_data[i] * self.weights[j][i]
    #         self.output_data[j] += self.bias[j]
    #     return self.output_data
    
    def forward(self, input_data):
        self.input_data = input_data
        self.output_data = np.dot(input_data, self.weights) + self.bias
        return self.output_data

    def backward(self, output_error, learning_rate):
        input_error = np.dot(output_error, self.weights.T)
        weights_error = np.dot(self.input_data.T, output_error)

        self.weights -= learning_rate * weights_error
        self.bias -= learning_rate * output_error
        return input_error
    