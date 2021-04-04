#include "classifier.h"
#include "mainwindow.h"

Classifier::Classifier(QObject *parent) : QThread(parent)
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

void Classifier::init(double learning_rate,
                      vector<int>& num_nodes,
                      vector<vector<double>>& input,
                      vector<double>& output,
                      int max_iterations)
{
    max_iterations_ = max_iterations;
    num_layers_ = num_nodes.size();
    learning_rate_ = learning_rate;
    is_learning_ = false;
    input_ = input;
    output_ = output;
    num_nodes_ = num_nodes;

    nodes_.clear();
    weights_.clear();
    derivatives_.clear();

    errors_.clear();
    predictions_.clear();

    nodes_.resize(num_layers_);
    weights_.resize(num_layers_-1);
    derivatives_.resize(num_layers_);
    int max_num_nodes = 0;
    for (int l = 0; l < num_layers_; l++) {
        if (l < num_layers_-1) {
            weights_[l] = vector<vector<double>> (num_nodes_[l], vector<double> (num_nodes_[l+1], 0.0));
            for (int i = 0; i < num_nodes_[l]; i++) {
                for (int j = 0; j < num_nodes_[l+1]; j++) {
                    weights_[l][i][j] = get_random_value(-2, 2);
                }
            }
        }

        derivatives_[l] = vector<double> (num_nodes_[l], 0.0);
        gradients_[l] = vector<double> (num_nodes_[l], 0.0);
        nodes_[l] = vector<double> (num_nodes_[l], 0.0);
        if (max_num_nodes < num_nodes[l])
            max_num_nodes = num_nodes[l];
    }

    for (int i = 0; i < 2; i++) {
        gradients_[i].clear();
        gradients_[i] = vector<double> (max_num_nodes, 0.0);
    }

    /* Index Information
     * num_nodes[l] := l번째 레이어의 노드 개수
     * nodes[l][i] := l번째 레이어의 i번째 노드의 계산 결과
     * weights[l][i][j] := l번째 레이어의 i번째 노드와 다음 레이어의 j번째 노드를 연결한 간선의 가중치
     * derivatives[l][i] := l번째 레이어의 i번째 노드의 계산 결과의 미분값
     * gradients[l][i] := l번째 레이어의 i번째 노드의 gradient
    */
}

Classifier::~Classifier()
{
    delete weight_info_;
}

bool Classifier::doing()
{
    return is_learning_;
}


void Classifier::change_state()
{
    is_learning_ = !is_learning_;
}


QString* Classifier::get_weight_info_str()
{
    return weight_info_;
}


double Classifier::get_random_value(int min, int max)
{
    static const double fraction = 1.0 / (RAND_MAX + 1.0);
    return min + static_cast<double>((max - min) * (rand() * fraction));
}


QString Classifier::get_weight_info()
{
    QString s;
    for (int l = 0; l < num_layers_ - 1; l++) {
        s.append("W");
        s.append(QString::number(l));
        s.append("\n");
        for (int i = 0; i < num_nodes_[l]; i++) {
            for (int j = 0; j < num_nodes_[l+1]; j++) {
                s.append(QString::number(weights_[l][i][j]));
                s.append(", ");
            }
            s.append("\n");
        }
        s.append("\n\n");
    }
    return s;
}


QString* Classifier::get_hypothesis_text() {
    int order_cnt = output_.size();
    QString *s = new QString("Hypothesis: [\n");
    for (int i=0; i<order_cnt; i++) {
        s->append("    ");
        s->append(QString::number(predictions_[i]));
        s->append(",\n");
    }
    s->append("]");
    return s;
}


void Classifier::debug(bool only_weight)
{
    if (!only_weight) {
        cout << "Layer Count : " << num_layers_ << endl;
        cout << "Node Count : ";
        for (int i = 0; i < num_layers_; i++)
            cout << num_nodes_[i] << " ";
        cout << endl;
    }

    cout << "Weight: " << endl;
    for (int l = 0; l < num_layers_ - 1; l++) {
        cout << "Layer " << l << " ~ Layer" << l+1 << endl;
        for (int i = 0; i < num_nodes_[l]; i++) {
            for (int j = 0; j < num_nodes_[l+1]; j++)
                cout << "W[" << i << "][" << j << "] = " << weights_[l][i][j] << ", \t";
            cout << endl;
        }
    }

    if (!only_weight) {
        cout << endl << "Input: " << endl;
        for (int i = 0; i < num_nodes_[0]; i++) {
            cout << nodes_[0][i] << " ";
        }
        cout << endl;

        for (int l = 1; l < num_layers_; l++) {
            cout << endl << "Layer " << l << " : " << endl;
            for (int i = 0; i < num_nodes_[l]; i++) {
                cout << nodes_[l][i] << ", ";
            }
            cout << endl;
        }
    }

    cout << endl << "Derivatives: " << endl;
    for (int l = 1; l < num_layers_; l++) {
        cout << endl << "Layer " << l << " : " << endl;
        for (int i = 0; i < num_nodes_[l]; i++) {
            cout << derivatives_[l][i] << ", ";
        }
        cout << endl;
    }
}


void Classifier::run()
{
    is_learning_ = true;
    bias_ = 1.;
    weight_info_ = new QString("[Initial Weight]\n");
    weight_info_->append(get_weight_info());
    predictions_ = vector<double> (output_.size(), 0.0);
    errors_ = vector<double> (max_iterations_);
    QThread::msleep(500);
    for (int iter = 0; iter < max_iterations_ and is_learning_; iter++) {
        double sse = 0.0;
        int num_errors = 0;
        for (int i = 0; i < output_.size(); i++) {
            for (int j = 0; j < input_.size(); j++) {
                nodes_[0][j] = input_[j][i];
            }
            predictions_[i] = forward();
            sse += loss(predictions_[i], output_[i]);
            if (fabs(predictions_[i] - output_[i]) > 0.03) {
                changed_weight(&weights_, &nodes_, bias_, iter, i, get_hypothesis_text());
                QThread:usleep(500);
                // 손실 함수의 미분 계산 (=출력층의 upstream gradient)
                double loss_gradient = loss_prime(nodes_[num_layers_ - 1][0], output_[i]);
                backward(loss_gradient);
                num_errors++;
            }
        }
        errors_[iter] = sse;
        if (num_errors == 0) break;
//        debug();
    }

//    debug();
    weight_info_->append("[Final Weight]\n");
    weight_info_->append(get_weight_info());
    done(errors_, is_learning_);
}


/* Return the result (=Wx) of dot product. */
double Classifier::dot_product(int prev_layer, int node) {
    double net = 0.0;
    // 현재 노드에 입력으로 들어오는 노드들의 계산 결과와 가중치를 곱해서 합침.
    for (int i = 0; i < num_nodes_[prev_layer]; i++) {
        double x = nodes_[prev_layer][i];
        double w = weights_[prev_layer][i][node];
        net += w * x;
    }
    return net;
}


/* Return the error value. */
double Classifier::loss(double prediction, double output)
{
    return pow(output - prediction, 2.0);
}


/* Return the derivative of loss function. */
double Classifier::loss_prime(double prediction, double output)
{
    return -2 * (output - prediction);
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

/* Return the prediction after forwarding. */
double Classifier::forward() {
    // 층마다 각 노드의 활성화 함수를 계산 (입력층 제외)
    for (int l = 1; l < num_layers_; l++) {
        for (int i = 0; i < num_nodes_[l]; i++) {
            double net = dot_product(l-1, i);
            nodes_[l][i] = sigmoid(net + bias_);
            // 미분 결과 캐싱
            derivatives_[l][i] = sigmoid_prime(nodes_[l][i]);
        }
    }
    return nodes_[num_layers_ - 1][0]; // 출력층 계산 결과 반환
}


/* Run back-propagation */
void Classifier::backward(double loss_gradient) {
    // 출력층의 gradient에 upstream gradient 저장
    gradients_[(num_layers_ - 1) & 1][0] = loss_gradient;

    for (int l = num_layers_ - 1; l > 0; l--) {
        for (int i = 0; i < num_nodes_[l]; i++) {
            // 현재 레이어의 gradient에 local gradient를 곱함.
            // upstream gradient 가 모두 더해졌다고 가정.
            gradients_[l & 1][i] *= derivatives_[l][i];

            // 현재 노드와 이전 레이어의 노드들과의 간선의 가중치 갱신 및 미분 결과 계산
            for (int j = 0; j < num_nodes_[l-1]; j++) {
                // 경사하강법: 파라미터는 미분 부호의 반대 방향으로 이동
                double delta_w = -learning_rate_ * gradients_[l & 1][i] * nodes_[l-1][j];
                weights_[l-1][j][i] += delta_w;

                // 입력으로 들어오는 노드의 gradient에 upstream gradient 추가 (downstream gradient 과정)
                gradients_[(l + 1) & 1][j] += gradients_[l & 1][i] * weights_[l-1][j][i];
            }
        }
    }
}
