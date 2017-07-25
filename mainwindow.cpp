#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(ui->graphicsView);
    builder = new CircuitBuilder(scene);
    this->setWindowTitle("Обработка схем по тройкам чисел");
}

MainWindow::~MainWindow()
{
    delete scene;
    delete ui;
    delete builder;
}

void MainWindow::on_pushButton_clicked()
{
    ui->graphicsView->scale(1.1, 1.1);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->graphicsView->scale(0.9, 0.9);
}

void MainWindow::on_pushButton_3_clicked()
{
    scene->clear();
    builder->clear();
    builder->build_circuit(ui->lineEdit->text(), 200, ui->verticalSlider_2->value());
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setScene(scene);

}

void MainWindow::on_spinBox_valueChanged(const QString &arg1)
{
    builder->change_graphics_pack("graphics"+arg1);
}

void MainWindow::on_pushButton_4_clicked()
{
    scene->clearSelection();
    scene->setSceneRect(scene->itemsBoundingRect());
    QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    scene->render(&painter);
    image.save(ui->lineEdit->text()+".png");
}
