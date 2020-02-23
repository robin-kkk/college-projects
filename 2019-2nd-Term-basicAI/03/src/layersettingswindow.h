#ifndef LAYERSETTINGSWINDOW_H
#define LAYERSETTINGSWINDOW_H

#include <QtWidgets>
#include <vector>

using namespace std;

class LayerSettingsWindow : public QWidget
{
    Q_OBJECT

public:
    LayerSettingsWindow(vector<int> *node_counts, QWidget *parent = nullptr)
    {
        _node_cnt = *node_counts;

        QVBoxLayout *layout = new QVBoxLayout();
        _top_layout = new QFormLayout();
        QHBoxLayout *bottom_layout = new QHBoxLayout();
        QPushButton *add_btn = new QPushButton("Add");
        QPushButton *remove_btn = new QPushButton("Remove");
        QPushButton *apply_btn = new QPushButton("Apply");

        qRegisterMetaType<vector<int>>("vector<int>");
        connect(add_btn, SIGNAL(clicked()), this, SLOT(add_layer()));
        connect(remove_btn, SIGNAL(clicked()), this, SLOT(remove_layer()));
        connect(apply_btn, SIGNAL(clicked()), this, SLOT(apply()));

        QLabel *notion_label = new QLabel(notion);
        layout->addWidget(notion_label);

        for (int i=0; i<_node_cnt.size(); i++)
        {
            QString s = layerLabel;
            s.append(QString::number(i+1));
            QLabel *label = new QLabel(s);
            label->setFixedWidth(labelWidth);

            QSpinBox *box = new QSpinBox();
            box->setObjectName(QString::number(i));
            box->setRange(2, 10);
            box->setValue(_node_cnt[i]);
            box->setFixedWidth(spinBoxWidth);
            box->setCursor(QCursor(Qt::PointingHandCursor));
            connect(box, SIGNAL(valueChanged(int)), this, SLOT(value_changed(int)));

            _top_layout->addRow(label, box);
        }

        _cur_layer_number = _node_cnt.size();

        bottom_layout->addWidget(add_btn, Qt::AlignCenter);
        bottom_layout->addWidget(remove_btn, Qt::AlignCenter);
        bottom_layout->addWidget(apply_btn, Qt::AlignCenter);
        layout->addLayout(_top_layout);
        layout->addLayout(bottom_layout);
        setWindowTitle(tr("Layer Setting"));
        setLayout(layout);
        show();
    }

    ~LayerSettingsWindow()
    {
        delete _top_layout;
    }

signals:
    void applied(vector<int> node_cnt);

public slots:
    void add_layer()
    {
        QString s = layerLabel;
        s.append(QString::number(_cur_layer_number+1));
        s.append(" : ");
        QLabel *label = new QLabel(s);
        label->setFixedWidth(labelWidth);

        QSpinBox *box = new QSpinBox();
        box->setObjectName(QString::number(_cur_layer_number));
        box->setRange(2, 10);
        box->setValue(3);
        box->setFixedWidth(spinBoxWidth);
        box->setCursor(QCursor(Qt::PointingHandCursor));
        connect(box, SIGNAL(valueChanged(int)), this, SLOT(value_changed(int)));

        _node_cnt.push_back(3);
        _top_layout->addRow(label, box);
        _cur_layer_number++;
        setFixedHeight(this->height() + heightGap);
    }

    void remove_layer()
    {
        _top_layout->removeRow(_cur_layer_number-1);
        _node_cnt.pop_back();
        _cur_layer_number--;
        setFixedHeight(this->height() - heightGap);
    }

    void apply()
    {
        _node_cnt.insert(_node_cnt.begin(), 2);
        _node_cnt.push_back(1);
        applied(_node_cnt);
    }

    void value_changed(int value)
    {
        QSpinBox *box = static_cast<QSpinBox*> (sender());
        QString name = box->objectName();
        _node_cnt[name.toInt()] = value;
    }

private:
    const QString notion = "[Default Setting]\nInput Layer\t:  2 nodes + 1 bias\nOutput Layer\t:  1 nodes\nHidden Layer\t:  3 nodes + 1 bias\n";
    const QString layerLabel = "Node Count in Hidden Layer ";
    const int spinBoxWidth = 60;
    const int labelWidth = 200;
    const int heightGap = 40;
    int _cur_layer_number = 0;
    vector<int> _node_cnt;
    QFormLayout *_top_layout;
};

#endif // LAYERSETTINGSWINDOW_H
