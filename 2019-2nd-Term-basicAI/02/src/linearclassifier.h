#ifndef LINEARCLASSIFIER_H
#define LINEARCLASSIFIER_H

#include <QThread>
#include <vector>

using namespace std;

class LinearClassifier : public QThread
{
    Q_OBJECT

signals:
    void changed_weight(double net, double x0, double w0, double x1, double w1);
    void done(vector<double> errors, vector<vector<double> > weights);

public:
    LinearClassifier(QObject *parent = nullptr);

    void ready(vector<vector<double> > &input,
               vector<int> &output,
               vector<double> &initial_weight,
               double learning_rate = 0.1);
    bool is_doing();
    void change_state();

private:
    double ZERO = 0.0;
    double net;
    double learning_rate;
    vector<vector<double> > input;
    vector<int> output;
    vector<vector<double> > weights;
    vector<double> errors;
    bool is_learning;

    void run();
    int calc(int input_order, int try_cnt);
};

#endif // LINEARCLASSIFIER_H
