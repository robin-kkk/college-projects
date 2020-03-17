# Multi-layer Perceptron  <!-- omit in toc -->

- Course: Artificial Intelligence
- Language: C++, Python

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
void changed_weight(vector<vector<vector<double> > > *weights,
                    vector<vector<vector<double> > > *inputs,
                    double bias,
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
double loss(double pred); // pred: output after training
double loss_prime(double pred);
double sigmoid(double x);
double sigmoid_prime(double f);

// Perform forwrarding during training.
double forward();

// Perform backwarding (Back-propagation)  during training
void update_weights();
```

### How it works

Before looking at the following, I recommend to understand the [backpropagation](http://cs231n.github.io/optimization-2/).

![6](./img/6.png?raw=true)

By `Chain Rule`, **Downstream gradient = (Upstream gradient) * (local gradient)**

![7](./img/7.png?raw=true)

Also, if a node has several output nodes, upstream gradients from them is all added up.

The important thing is that upstream gradients that each node in a layer got can be equal. So, the computation can be decreased with dynamic programming.

The following code reflects these points.

``` cpp
/* Index Information
  * - _node_count[i] : `i` is the layer number. (0 = Input Layer, Last Layer = Output Layer)
  * - _input[i][j][k] : `i` is the layer number. 'j' is the node number per layer.
  *                    `k` is input order, but hidden layer and output layer have only 0.
  * - _output[k] : `k` is input order. (ex: in AND Gate, { 0, 0, 0, 1 })
  * - _weight[i][j][k] : `i` is the layer number. (Save weight matrix passed from `i` to the `i+1` th.)
  *                      `j` is the node number in `i+1`th layer.
  *                      `k` is the node number in `i`th layer.
  * - _derivative[i][j] : `i` is the layer number starting at the hidden layer.
  *                       `j` is a node number in `i`th layer. Last element is derivative value of output layer.
  * - _error[i] : `i` is the number of attempts.
  * - _delta[i][j] : `i` is the layer number. `j` is the node number in `i`th layer.
*/
```

``` cpp
/* Return the predicted value after forwarding. */
double Classifier::forward()
{
  double f = 0.0;
  for (int i=1; i<_layer_count; i++)
  {
    int current_node_count = _node_count[i];
    for (int j=0; j<current_node_count; j++)
    {
      double net = dot_product(i-1, j);
      f = sigmoid(net + _bias);
      _input[i][j][0] = f;
      _derivative[i-1][j] = sigmoid_prime(f);
    }
  }

  _derivative[_layer_count-1][0] = loss_prime(f); // dE_dO
  return f;
}
```

``` cpp
void Classifier::update_weights()
{
  _delta[_layer_count-1][0] = _derivative[_layer_count-1][0]; // dE_dO

  for (int l=_layer_count-1; l>0; l--)
  {
    int order = (l == 1) ? _input_order : 0;
    int node_cnt_in_curr_layer = _node_count[l];
    int node_cnt_in_input_layer = _node_count[l-1];
    for (int i=0; i<node_cnt_in_curr_layer; i++)
    {
      _delta[l][i] *= _derivative[l-1][i]; // Calculate local graidents
      for (int j=0; j<node_cnt_in_input_layer; j++)
      {
        // Calculate upstream gradient and save.
        _delta[l-1][j] += _delta[l][i] * _weight[l-1][i][j];
        // update weight of each input node.
        double delta_w = -_learning_rate * _delta[l][i] * _input[l-1][j][order];
        _weight[l-1][i][j] += delta_w;
      }
    }
  }
}
```

## Conclusion

The things I missed are about optimization, such as Weight Initialization and so on. In this case, this program might not train since the weight is initialized randomly. Also, the experiment had been done by the suggested data only.

Finally, I hope you think this focuses on implementing backpropagation.
