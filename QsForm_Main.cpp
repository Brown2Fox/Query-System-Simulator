#include "QsForm_Main.hpp"
#include "ui_QsForm_Main.h"

#include <QDebug>

#include "QsForm_Settings.hpp"
#include "QsThreadCore.hpp"
#include "QsSettings.hpp"
#include "QsRes.hpp"


QsForm_Main::QsForm_Main(QWidget* parent) : QDialog(parent), ui(new Ui::QsForm_Main)
{
    ui->setupUi(this);

    ui->btStart->setIcon(QIcon("play.png"));
    ui->btBreak->setIcon(QIcon("stop.png"));
    ui->btNext->setIcon(QIcon("next.png"));
    ui->btSettings->setIcon(QIcon("set.png"));


    ui->hframe_step_all->hide();

    ui->tbAuto1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tbAuto2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tbStepEvCal->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    clear_tables();

}

QsForm_Main::~QsForm_Main()
{
    delete ui;
}

void QsForm_Main::auto_start()
{

    res = new QsRes();
    core = new QsThreadCore();

    core->start_auto();
    res->calculateSummaryInfo();

    this->output_res_auto();

    delete res;
    delete core;
}

void QsForm_Main::byStep_start()
{

    res = new QsRes();
    core = new QsThreadCore();
}


void QsForm_Main::on_btStart_clicked()
{

    if (QsSettings::byStep)
    {
        ui->btStart->setEnabled(false);
        ui->btNext->setEnabled(true);
        ui->btBreak->setEnabled(true);
        ui->cbByStep->setEnabled(false);
        ui->btSettings->setEnabled(false);


        byStep_start();

    }
    else
    {
        auto_start();
    }

}

void QsForm_Main::on_btNext_clicked()
{
    if (core->step()) {

        res->calculateSummaryInfo();

        ui->btStart->setEnabled(true);
        ui->btNext->setEnabled(false);
        ui->btBreak->setEnabled(false);
        ui->cbByStep->setEnabled(true);
        ui->btSettings->setEnabled(true);

        output_res_step();
        output_res_auto();
        qDebug() << "print step end";
        delete core;
        delete res;
    } else {

        res->calculateSummaryInfo();
        output_res_step();
        output_res_auto();
        qDebug() << "print step";
    }
}

void QsForm_Main::on_cbByStep_stateChanged(int state)
{
    QsSettings::byStep = (bool)state;
    if (QsSettings::byStep) {
        ui->vframe_auto_all->hide();
        ui->hframe_step_all->show();
    }
    else {
        ui->hframe_step_all->hide();
        ui->vframe_auto_all->show();
    }
}

void QsForm_Main::on_btBreak_clicked()
{
        ui->btStart->setEnabled(true);
        ui->btNext->setEnabled(false);
        ui->btBreak->setEnabled(false);
        ui->cbByStep->setEnabled(true);
        ui->btSettings->setEnabled(true);

        if (core != nullptr) delete core;
        if (res != nullptr) delete res;
}

void QsForm_Main::on_btSettings_clicked()
{
    connect(set_win, SIGNAL(settings_changed_signal()),this, SLOT(clear_tables()) );

    set_win->show();
}

void QsForm_Main::clear_tables()
{
    int& C = QsSettings::C;
    int& Q = QsSettings::Q;
    int& S = QsSettings::S;

    int count = ui->tbAuto1->rowCount();
    for (int i = 0; i < count; ++i) { ui->tbAuto1->removeRow(0); }
    count = ui->tbAuto2->rowCount();
    for (int i = 0; i < count; ++i) { ui->tbAuto2->removeRow(0); }
    count = ui->tbStepEvCal->rowCount();
    for (int i = 0; i < count; ++i) { ui->tbStepEvCal->removeRow(0); }
    count = ui->tbStepQue->rowCount();
    for (int i = 0; i < count; ++i) { ui->tbStepQue->removeRow(0); }


    ui->tbAuto1->setRowCount(C);
    QStringList list_str;
    for (int i = 0; i < C; ++i) {

        list_str << QString().asprintf("Cl%i", i+1);
        for (int j = 0; j < ui->tbAuto1->columnCount(); ++j) { ui->tbAuto1->setItem(i,j,new QTableWidgetItem("")); }
    }
    ui->tbAuto1->setVerticalHeaderLabels(list_str);

    ui->tbAuto2->setRowCount(S);
    list_str.clear();
    for (int i = 0; i < S; ++i) {

        list_str << QString().asprintf("Sr%i", i+1);
        for (int j = 0; j < ui->tbAuto2->columnCount(); ++j) { ui->tbAuto2->setItem(i,j,new QTableWidgetItem("")); }
    }
    ui->tbAuto2->setVerticalHeaderLabels(list_str);


    ui->tbStepEvCal->setRowCount(C + S);
    list_str.clear();
    for (int i = 0; i < C; ++i) { list_str << QString().asprintf("Cl%i", i+1); }
    for (int i = C; i < C+S; ++i) { list_str << QString().asprintf("Sr%i", i+1-C); }
    for (int i = 0; i < C+S; ++i) {
        for (int j = 0; j < ui->tbStepEvCal->columnCount(); ++j) { ui->tbStepEvCal->setItem(i,j,new QTableWidgetItem("")); }
    }
    ui->tbStepEvCal->setVerticalHeaderLabels(list_str);

    ui->tbStepQue->setRowCount(Q);
    list_str.clear();
    for (int i = 0; i < Q; ++i) {

        list_str << QString().asprintf("Qu%i", i+1);
        for (int j = 0; j < ui->tbStepQue->columnCount(); ++j) { ui->tbStepQue->setItem(i,j,new QTableWidgetItem("")); }
    }
    ui->tbStepQue->setVerticalHeaderLabels(list_str);


}

void QsForm_Main::output_res_auto()
{
    for (int c = 0; c < QsSettings::C; ++c) {

        QTableWidgetItem* item = nullptr;
        auto& info = QsRes::sumInfoByClient[c];

        item = ui->tbAuto1->item(c,0); Q_CHECK_PTR(item);
        item->setText(QString().asprintf("%i",info.total));

        item = ui->tbAuto1->item(c,1); Q_CHECK_PTR(item);
        item->setText(QString().asprintf("%i", info.rejected));

        item = ui->tbAuto1->item(c,2); Q_CHECK_PTR(item);
        item->setText(QString().asprintf("%.4f", info.p_rej));

        item = ui->tbAuto1->item(c,3); Q_CHECK_PTR(item);
        item->setText(QString().asprintf("%.4f", info.mean_t_resid));

        item = ui->tbAuto1->item(c,4); Q_CHECK_PTR(item);
        item->setText(QString().asprintf("%.4f", info.mean_t_wait));

        item = ui->tbAuto1->item(c,5); Q_CHECK_PTR(item);
        item->setText(QString().asprintf("%.4f", info.mean_t_proc));

        item = ui->tbAuto1->item(c,6); Q_CHECK_PTR(item);
        item->setText(QString().asprintf("%.4f", info.disp_t_wait));

        item = ui->tbAuto1->item(c,7); Q_CHECK_PTR(item);
        item->setText(QString().asprintf("%.4f", info.disp_t_proc));


    }

    for (int s = 0; s < QsSettings::S; ++s) {

        QTableWidgetItem* item = nullptr;
        auto& info = QsRes::sumInfoByServer[s];


        item = ui->tbAuto2->item(s,0); Q_CHECK_PTR(item);
        item->setText(QString().asprintf("%.4f", info.k_usage));

    }

}

void QsForm_Main::output_res_step()
{
         const auto& event = core->eventScope.getCurrentObj();
         for (auto& cl : core->clientScope.getScope() ) {

             double time = cl->getTime();
             char ind_ev = (cl == event) ? '*' : ' ';

             int c = cl->getId() - 1;
             auto& info = QsRes::sumInfoByClient[c];

             QTableWidgetItem* item = nullptr;

             item = ui->tbStepEvCal->item(c,0); Q_CHECK_PTR(item);
             item->setText(QString().asprintf("%c", ind_ev));

             item = ui->tbStepEvCal->item(c,1); Q_CHECK_PTR(item);
             item->setText(QString().asprintf("%.3f", time));

             item = ui->tbStepEvCal->item(c,2); Q_CHECK_PTR(item);
             item->setText(QString().asprintf("%i", info.accepted));

             item = ui->tbStepEvCal->item(c,3); Q_CHECK_PTR(item);
             item->setText(QString().asprintf("%i", info.rejected));
         }

         for (auto& srv : core->serverScope.getScope() ) {

             double time = srv->getTime();
             char ind_ev = (srv == event) ? '*' : ' ';

             int s = srv->getId() - 1;
             int c = QsSettings::C;

             QTableWidgetItem* item = nullptr;

             item = ui->tbStepEvCal->item(c + s,0); Q_CHECK_PTR(item);
             item->setText(QString().asprintf("%c", ind_ev));

             item = ui->tbStepEvCal->item(c + s,1); Q_CHECK_PTR(item);
             item->setText(QString().asprintf("%.3f", time));

         }

        for (auto& qu : core->queueScope.getScope() ) {

            double time = (qu->hasQuery()) ? qu->getTime() : 0;
            int id = (qu->hasQuery()) ? qu->getQuery()->getIdCl() : 0;
            int count = (qu->hasQuery()) ? qu->getQuery()->getCount() : 0;

            int q = qu->getId() - 1;

            QTableWidgetItem* item = nullptr;

            item = ui->tbStepQue->item(q, 0); Q_CHECK_PTR(item);
            item->setText(QString().asprintf("%.3f", time));

            item = ui->tbStepQue->item(q, 1); Q_CHECK_PTR(item);
            item->setText(QString().asprintf("%i", id));

            item = ui->tbStepQue->item(q, 2); Q_CHECK_PTR(item);
            item->setText(QString().asprintf("%i", count));


        }

}




