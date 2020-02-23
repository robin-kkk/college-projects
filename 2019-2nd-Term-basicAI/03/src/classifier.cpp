#include "classifier.h"
#include "mainwindow.h"

Classifier::Classifier(QObject *parent) : QThread(parent)
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

void Classifier::ready(vector<int> node_count,
                       double learning_rate,
                       vector<vector<double> > input,
                       vector<double> output,
                       int max_iter_cnt)
{
    _max_iter_cnt = max_iter_cnt;
    _layer_count = node_count.size();
    _learning_rate = learning_rate;
    _learning = false;

    /* Initialize Input/Output/Error vector. */
    _node_count.clear();
    _input.clear();
    _output.clear();
    _weight.clear();
    _derivative.clear();
    _delta.clear();
    _error.clear();
    _input.push_back(input); // Set Xs in input layer.
    _output = output;
    _pred.resize(_output.size());

    _node_count = node_count;
    _weight.resize(_layer_count-1);
    _derivative.resize(_layer_count);
    _delta.resize(_layer_count);
    _delta[_layer_count-1] = vector<double> {0.};
    _derivative[_layer_count-1] = vector<double> {0.}; // Initialize the derivative value in output layer.

    /* Initialize required values.
     * - the number of node per layer
     * - input value (Xs), derivative values with zero per node
     * - weight-matrix with random value
    */
    for (int i=1; i<_layer_count; i++)
    {
        int node_cnt_in_prev_layer = _node_count[i-1];
        int node_cnt_in_cur_layer = _node_count[i];
        vector<double> v1(node_cnt_in_cur_layer, 0.0);
        _derivative[i-1] = v1;

        vector<double> v2(node_cnt_in_cur_layer, 0.0);
        _delta[i-1] = v2;

        vector<vector<double> > input_matrix;
        input_matrix.resize(node_cnt_in_cur_layer, vector<double> {0.0});
        _input.push_back(input_matrix);

        vector<vector<double> > weight_matrix(node_cnt_in_cur_layer);
        vector<double> weight_row(node_cnt_in_prev_layer);
        for (int j=0; j<node_cnt_in_cur_layer; j++)
        {
            for (int k=0; k<node_cnt_in_prev_layer; k++)
            {
                weight_row[k] = get_random_value(-2, 2);
            }
            weight_matrix[j] = weight_row;
        }
        _weight[i-1] = weight_matrix;
    }


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
}

Classifier::~Classifier() {
    delete weight_info;
}

bool Classifier::doing()
{
    return _learning;
}


void Classifier::change_state()
{
    _learning = !_learning;
}


QString* Classifier::get_weight_info_str()
{
    return weight_info;
}


double Classifier::get_random_value(int min, int max)
{
    static const double fraction = 1.0 / (RAND_MAX + 1.0);
    return min + static_cast<double>((max - min) * (rand() * fraction));
}


QString Classifier::get_weight_info()
{
    QString s;

    for (int i=0; i<_layer_count-1; i++)
    {
        s.append("W");
        s.append(QString::number(i));
        s.append("\n");
        int curr_node_cnt = _node_count[i];
        int next_node_cnt = _node_count[i+1];
        for (int r=0; r<next_node_cnt; r++)
        {
            for(int c=0; c<curr_node_cnt; c++)
            {
                s.append(QString::number(_weight[i][r][c]));
                s.append(", ");
            }
            s.append("\n");
        }
        s.append("\n\n");
    }
    return s;
}


void Classifier::debug(bool only_weight)
{
    if (!only_weight) {
        cout << "Layer Count : " << _layer_count << endl;
        cout << "Node Count : ";
        for (int i=0; i<_layer_count; i++)
            cout << _node_count[i] << " ";
        cout << endl;
    }

    cout << "Weight: " << endl;
    for (int i=0; i<_layer_count-1; i++)
    {
        vector<vector<double> > matrix = _weight[i];
        int next_cnt = _node_count[i+1];
        int curr_cnt = _node_count[i];
        cout << "Layer " << i << " ~ Layer" << i+1 << endl;
        for (int r=0; r<next_cnt; r++)
        {
            for (int c=0; c<curr_cnt; c++)
            {
                cout << "W[" << r << "][" << c << "] = " << matrix[r][c] << ",\t";
            }
            cout << endl;
        }
    }

    if (!only_weight)
    {
        cout << endl << "X in Input: " << endl;
        vector<vector<double> > matrix = _input[0];
        for (int i=0; i<matrix.size(); i++)
        {
            cout << "x" << i << " = ";
            for (int j=0; j<matrix[0].size(); j++) // order
            {
                cout << matrix[i][j] << ", ";
            }
            cout << endl;
        }


        for (int i=1; i<_layer_count; i++)
        {
            cout << endl << "X of Node in L" << i << ": " << endl;
            vector<vector<double> > matrix = _input[i];
            for (int j=0; j<_node_count[i]; j++)
            {
                cout << "x" << j << " = " << matrix[j][0] << ",  ";
            }
            cout << endl;
        }
    }

    cout << endl << "Derivative: " << endl;
    for (int i=0; i<_layer_count-1; i++)
    {
        cout << "Layer: " << i+1 << ", Number of nodes: " << _node_count[i+1] << endl;
        for (int j=0; j<_node_count[i+1]; j++)
        {
            cout << _derivative[i][j] << endl;
        }
    }
}

void Classifier::run()
{
    _learning = true;
    _bias = 1.;
    int order_cnt = _output.size();
    int i = 0;
    weight_info = new QString("[Initial Weight]\n");
    weight_info->append(get_weight_info());
    QThread::msleep(500);
    while (_learning)
    {
        if (i == _max_iter_cnt) break;
        int e_cnt = 0;
        double e_sum = 0.;
        for (_input_order=0; _input_order<order_cnt; _input_order++)
        {
            _pred[_input_order] = forward();
            double e = loss(_pred[_input_order]);
            if (fabs(_output[_input_order] - _pred[_input_order]) > 0.03) {
                e_cnt++;
                changed_weight(&_weight, &_input, _bias, get_hypothesis_text());
                QThread:usleep(500);
                update_weights();
            }
            e_sum += e;
        }

        _error.push_back(e_sum);
        if (!e_cnt) {
            break;
        }

        i++;
    }

//    debug();
    weight_info->append("[Final Weight]\n");
    weight_info->append(get_weight_info());
    done(_error, _learning);
}

QString* Classifier::get_hypothesis_text() {
    int order_cnt = _output.size();
    QString *s = new QString("Hypothesis: [\n");
    for (int i=0; i<order_cnt; i++) {
        s->append("    ");
        s->append(QString::number(_pred[i]));
        s->append(",\n");
    }
    s->append("]");
    return s;
}

/* Return the result (=Wx) of dot product. */
double Classifier::dot_product(int prev_layer, int node_idx)
{
    int order = (prev_layer == 0) ? _input_order : 0;
    int node_cnt = _node_count[prev_layer];
    double net = 0.0;

    for (int i=0; i<node_cnt; i++)
    {
        double x = _input[prev_layer][i][order];
        double w = _weight[prev_layer][node_idx][i];
        net += w*x;
    }

    return net;
}


/* Return the error value. */
double Classifier::loss(double pred)
{
    return pow(_output[_input_order] - pred, 2.0);
}


/* Return the derivative of loss function. */
double Classifier::loss_prime(double pred)
{
    return -2 * (_output[_input_order] - pred);
}


/* Return sigmoid value */
double Classifier::sigmoid(double x)
{
    return (1.0 / (1.0 + exp(-x)));
}

double Classifier::sigmoid_prime(double f)
{
    return (1 - f) * f;
}

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


/* Update all of weights. */
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
            _delta[l][i] *= _derivative[l-1][i];
            for (int j=0; j<node_cnt_in_input_layer; j++)
            {
                _delta[l-1][j] += _delta[l][i] * _weight[l-1][i][j];
                double delta_w = -_learning_rate * _delta[l][i] * _input[l-1][j][order];
                _weight[l-1][i][j] += delta_w;
            }
        }
    }
}
