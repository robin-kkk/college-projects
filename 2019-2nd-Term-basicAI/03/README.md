# Multi-layer Perceptron  <!-- omit in toc -->

- Course: Artificial Intelligence
- Language: C++

This is a very simple machine learning program.

---

#### Table of contents <!-- omit in toc -->

- [Overview](#overview)
- [User Interface](#user-interface)
- [Classifier Class](#classifier-class)
  - [Members](#members)
  - [How it works](#how-it-works)
- [Conclusion](#conclusion)

---

## Overview

- Language: C++
- Library: Qt 5.13
- Class
  - **MainWindow.** helps a user select options necessary for training.
  - **LayerSettingsWindow.** sets the configuration of the layers in a neural network.
  - **LearningGraph.** shows a linear relationship between a node in a layer and two input nodes connected to the node as a line in 2 dimension space every iteration.
  - **ReplayWindow.** shows again learning graphs about nodes of all layers except input layer.
  - **ReplayThread.** displays data, got during training, on LearningGraph.
  - **Classifier.** trains data configured by a user.

## User Interface

![1](./img/1.png?raw=true)

- A learning graph of the left top is about the output layer, which has two input nodes from the last hidden layer.
- A vertical axis is of X1 and a horizontal axis is of X0. 
- Also, a user can see again all graphs during training after it.

![2](./img/2.png?raw=true)

- If the button of the right top, `Weight`, is pressed, open the `Weight Matrix` window, which shows the initial and final weight as a matrix format.
- This information can be saved as a txt file, too.

![3](./img/3.png?raw=true)

- If the button, `Set Node Count`, is pressed, open the `Layer Setting` window in which each layer is added or removed and the count of nodes is set.

![4](./img/4.png?raw=true)

- If the button, `See More Graphs`, is pressed, open the replay window, which shows learning graphs of all layers.
- If the button, `Clear`, is pressed, all user interfaces are initialized. If not, error graphs is overlapped whenever the next training ends. Just like below.

![5](./img/5.png?raw=true)

## Classifier Class

This is a thread class to implement `Multi-Layer Perceptron`.

- [classifier.h](./src/classifier.h)
- [classifier.cpp](./src/classifier.cpp)

### Members

I would introduce the important members.

- _signals_

``` cpp
// Send weight, input, bias to MainWindow class after backpropagation.
void changed_weight(vector<vector<vector<double>>>* weights_,
                    vector<vector<double>>* nodes_,
                    double bias, int iter, int input_order,
                    QString* h_text);

// Send error data to MainWindow class after training.
void done(vector<double> errors, bool not_broken);
```

- _methods_

```cpp
/*
Matrix multiplication of weight and input node.
- prev_layer: the index of the input layer, which is the previous layer.
- node_idx: the index of the output node, which exists in the current layer.
*/
double dot_product(int prev_layer, int node_idx);

// Some utility functions
double loss(double prediction, double output);
double loss_prime(double prediction, double output);
double sigmoid(double x);
double sigmoid_prime(double f);

// Perform forwrarding during training.
double forward();

// Perform backwarding (Back-propagation)  during training
void backward(double loss_gradient);
```

### How it works

Before looking at the following, I recommend to understand the [backpropagation](http://cs231n.github.io/optimization-2/).

![6](./img/6.png?raw=true)

By `Chain Rule`, **Downstream gradient = SUM(Upstream gradient) * (local gradient) * weight**

![7](./img/7.png?raw=true)

Also, if a node has several output nodes, upstream gradients from them is all added up.

The important thing is that upstream gradients that each node in a layer got can be equal. So, the computation can be decreased with dynamic programming.

The following code reflects these points.

``` cpp
/* Index Information
 * num_nodes[l] := # of nodes in l-th layer.
 * nodes[l][i] := activation function result of i-th node in l-th layer.
 * weights[l][i][j] := weight between i-th node in l-th layer and j-th node in (l+1)-th layer.
 * gradients[2][i] := gradient of i-th node in current layer and prev layer.
*/
```

``` cpp
/* Return the prediction after forwarding. */
double Classifier::forward() {
    for (int l = 1; l < num_layers_; l++) {
        for (int i = 0; i < num_nodes_[l]; i++) {
            double net = dot_product(l-1, i);
            nodes_[l][i] = sigmoid(net + bias_);
        }
    }
    return nodes_[num_layers_ - 1][0];
}

/* Run back-propagation */
void Classifier::backward(double loss_gradient) {
    // Assign gradient of loss function as gradient of output layer (1-node)
    gradients_[(num_layers_ - 1) & 1][0] = loss_gradient;

    for (int l = num_layers_ - 1; l > 0; l--) {
        for (int i = 0; i < num_nodes_[l]; i++) {
            // Multiply local gradient to gradient of current layer.
            // Assume that it calculated already downstream gradient from next layer.
            // -> downstream gradient = SUM(upstream gradient) * local gradient * weight.
            gradients_[l & 1][i] *= sigmoid_prime(nodes_[l][i]);

            for (int j = 0; j < num_nodes_[l-1]; j++) {
                // Calculate change of change.
                // Note that, in gradient descent, the parameter should be moved by reverse direction of gradient scope.
                double delta_w = -learning_rate_ * gradients_[l & 1][i] * nodes_[l-1][j];
                weights_[l-1][j][i] += delta_w;

                // Propagate downstream gradient.
                // For gradient of previous layer, add upstream gradient of the current node to gradient of each node of the previous layer.
                gradients_[(l + 1) & 1][j] += gradients_[l & 1][i] * weights_[l-1][j][i];
            }
        }
    }
}

```

## Conclusion

The things I missed are about optimization, such as Weight Initialization and so on. In this case, this program might not train since the weight is initialized randomly. Also, the experiment had been done by the suggested data only.

Finally, I hope you think this focuses on implementing backpropagation.
