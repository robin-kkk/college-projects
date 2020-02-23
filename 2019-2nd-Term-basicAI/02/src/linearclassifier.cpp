#include "linearclassifier.h"
#include "mainwindow.h"

LinearClassifier::LinearClassifier(QObject *parent) : QThread(parent) {}

void LinearClassifier::ready(vector<vector<double> > &input,
                            vector<int> &output,
                            vector<double> &initial_weight,
                             double learning_rate)
{
    this->input.resize(0);
    this->output.resize(0);
    this->weights.resize(0);
    this->errors.resize(0);

    this->input = input;
    this->output = output;
    this->weights.push_back(initial_weight);
    this->learning_rate = learning_rate;
}


bool LinearClassifier::is_doing()
{
    return is_learning;
}


void LinearClassifier::change_state()
{
    is_learning = !is_learning;
}


void LinearClassifier::run()
{
    int try_cnt = 0;
    is_learning = true;

    while (is_learning)
    {
        bool completed = true;
        int input_order;
        for (input_order=0; input_order<4; input_order++)
        {
            int current_output = calc(input_order, try_cnt);

            if (current_output != output[input_order])
            {
                double target = static_cast<double>(output[input_order]);
                double pred = static_cast<double>(current_output);
                double delta = target - pred;
                vector<double> current_weight;

                for (int i=0; i<input.size(); i++)
                {
                    double x = input[i][input_order];
                    double w = weights[try_cnt][i];
                    w += learning_rate * delta * x;
                    current_weight.push_back(w);
                }

                weights.push_back(current_weight);
                errors.push_back(delta);
                completed = false;
                break;
            }
        }

        if (completed)
        {
            input_order--;
        }

        double x0 = input[0][input_order];
        double x1 = input[1][input_order];
        double w0 = weights[try_cnt][0];
        double w1 = weights[try_cnt][1];

        changed_weight(net, x0, w0, x1, w1);
        QThread::msleep(200);
        try_cnt++;

        if (completed)
        {
            errors.push_back(0);
            done(errors, weights);
            break;
        }
    }    
}


int LinearClassifier::calc(int input_order, int try_cnt)
{
    net = 0.0;

    for (int i=0; i<input.size(); i++)
    {
        double x = input[i][input_order];
        double w = weights[try_cnt][i];
        net += w*x;
    }

    return (net > ZERO) ? 1 : 0;
}
