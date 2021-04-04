#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <QThread>

#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>

using namespace std;

class Classifier : public QThread
{
    Q_OBJECT

signals:
    void changed_weight(vector<vector<vector<double>>>* weights,
                        vector<vector<double>>* nodes,
                        double bias, int iter, int input_order,
                        QString* h_text);
    void done(vector<double> errors, bool not_broken);

public:
    const vector<vector<double> > sampleInputGate = {
        { 0., 0., 1., 1. }, { 0., 1., 0., 1. }
    };
    const vector<double> sampleOutputAND = { 0., 0., 0., 1. };
    const vector<double> sampleOutputOR = { 0., 1., 1., 1. };
    const vector<double> sampleOutputXOR = { 0., 1., 1., 0. };
    const vector<vector<double> > sampleInputDonut = {
        { 0., 0., 1., 1., 0.5, 1., 0., 0.5, 0.5 },
        { 0., 1., 0., 1., 1., 0.5, 0.5, 0., 0.5 }
    };
    const vector<double> sampleOutputDonut = { 0., 0., 0., 0., 0., 0., 0., 0., 1. };

    Classifier(QObject *parent = nullptr);
    ~Classifier();

    void init(double learning_rate,
              vector<int>& num_nodes,
              vector<vector<double>>& input,
              vector<double>& output,
              int max_iterations = 10000);

    bool doing();
    void change_state();
    QString* get_weight_info_str();

private:
    QString* weight_info_ = nullptr;
    bool is_learning_;
    int max_iterations_;
    int num_layers_;
    double learning_rate_;
    double bias_;
    vector<vector<double>> input_;
    vector<double> output_;
    vector<int> num_nodes_;
    vector<vector<double>> nodes_;
    vector<vector<vector<double>>> weights_;
    vector<double> gradients_[2];
    vector<double> errors_;
    vector<double> predictions_;

    void debug(bool only_weight = false);
    void run();
    QString* get_hypothesis_text();
    QString get_weight_info();
    double get_random_value(int min, int max);
    double dot_product(int prev_layer, int node_idx);
    double loss(double prediction, double output);
    double loss_prime(double prediction, double output);
    double sigmoid(double x);
    double sigmoid_prime(double f);
    double forward();
    void backward(double loss_gradient);
};

#endif // LINEARCLASSIFIER_H
