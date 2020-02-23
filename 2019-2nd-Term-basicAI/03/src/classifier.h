#ifndef LINEARCLASSIFIER_H
#define LINEARCLASSIFIER_H

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
    void changed_weight(vector<vector<vector<double> > > *weights,
                        vector<vector<vector<double> > > *inputs,
                        double bias,
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

    void ready(vector<int> node_count,
               double learning_rate,
               vector<vector<double> > input,
               vector<double> output,
               int max_iter_cnt = 10000);

    bool doing();
    void change_state();
    QString *get_weight_info_str();

private:
    QString *weight_info = nullptr;
    bool _learning;
    int _max_iter_cnt;
    int _input_order;
    int _layer_count;
    double _learning_rate;
    double _bias;
    vector<int> _node_count;
    vector<vector<vector<double> > > _input;
    vector<double> _output;
    vector<vector<vector<double> > > _weight;
    vector<vector<double> > _derivative;
    vector<vector<double> > _delta;
    vector<double> _error;
    vector<double> _pred;

    void debug(bool only_weight = false);
    void run();
    QString* get_hypothesis_text();
    QString get_weight_info();
    double get_random_value(int min, int max);
    double dot_product(int prev_layer, int node_idx);
    double loss(double pred);
    double loss_prime(double pred);
    double sigmoid(double x);
    double sigmoid_prime(double f);
    double forward();
    void update_weights();
};

#endif // LINEARCLASSIFIER_H
