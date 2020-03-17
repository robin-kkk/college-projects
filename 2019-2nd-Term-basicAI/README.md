# basic AI

- Course: Artificial Intelligence
- Language: C++, Python

There are 4 side projects inside. (just programming assignments)

- [`01`](./01) - _Single-layer Perceptron with training AND gate_
  - Condition
    1. Initialize Weight values (w[n] and theta) to random.
    2. Get the count of wrong outputs from 4 input in AND Gate.
    3. Exit if training correct.
    4. Weight is changed to the entered values.
  - Language: C/C++

- [`02`](./02) - _Single-layer Perceptron with training AND, OR, XOR gate_
  - Condition
    1. Show learning process as a graph with 2D (X1, X2 axis).
    2. Show error rate per interation. as a graph.
    3. Implement by modules.
  - Language: C/C++

- [`03`](./03) - _Multi-layer Perceptron_
  - Condition
    1. input: AND, OR, XOR gate + `Donut shape`

        ```c
        // Donut data
        float train_set_x[][2] = {{0.,0.},
            {0.,1.},
            {1.,0.},
            {1.,1.},
            {0.5,1.},
            {1.,0.5},
            {0.,0.5},
            {0.5,0.},
            {0.5,0.5}} ;
        float train_set_y[] = {0,0,0,0,0,0,0,0,1} ;
        ```

    2. Declare layer count, node count per layer as a Variable.
    3. Save weight as a file.
    4. Define each node of a neural network as a linear relation.
    5. Show learning process as a graph with 2D (X1, X2 axis).
    6. Show error rate per interation. as a graph.
  - Language: C/C++

- [`04`](./04) - _Improving performance of MINIST_
  - Condition
    1. Modify [the suggested code](https://github.com/Jpub/TensorflowDeeplearning/blob/master/Chapter05/MNIST%20double%20layer%20CNN%20classification.ipynb)
    2. Improve accuracy less than 1% against test data.
    3. Extend the given source by various methods.
    4. Increase the count of Convolution Hidden layer.
    5. Filter size = 3x3
  - Language: Python with tensorflow
