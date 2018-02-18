#include "testwindow.h"
#include "ui_testwindow.h"

TestWindow::TestWindow(QWidget *parent): QDialog(parent),  ui(new Ui::TestWindow)
{
   ui->setupUi(this);
}

void TestWindow::showWindow()
{
    this->show();
    showQuestion(0);
    ui->imageLabel->hide();
}

void TestWindow::setQuestions(QVector<Question> _questions)
{
    questions = _questions;
    currentQuestion = 0;
}

void TestWindow::on_nextButton_clicked()
{
    if(currentQuestion == questions.size()-1) return;
    hideQuestion();
    currentQuestion++;
    showQuestion(currentQuestion);
}

void TestWindow::on_prevButton_clicked()
{
    if(currentQuestion == 0) return;
    hideQuestion();
    currentQuestion--;
    showQuestion(currentQuestion);
}

void TestWindow::on_submitButton_clicked()
{
    hideQuestion();
    Network & network = Network::getInstance();
    float mark = network.sendQuestions(questions);
    infowindow * infoWindow = new infowindow(0,QString("\nВаша оценка: ") +  QString::number(mark));
    infoWindow->show();
    //ui->label->setText(QString("\nВаша оценка: ") +  QString::number(mark));
    this->close();
}

void TestWindow::showQuestion(int number)
{

    if ( ! questions[currentQuestion].getAdvancedData().isEmpty()){ //показать изображение если есть
        QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData( questions[currentQuestion].getAdvancedData() );
            int h = ui->imageLabel->height();
            int w = ui->imageLabel->width();
            ui->imageLabel->setPixmap(outPixmap.scaled(w,h,Qt::KeepAspectRatio,Qt::SmoothTransformation));
            ui->imageLabel->show();
    }
    int type = questions[number].type;
    ui->label->setText(questions[number].getQuestionText());
    if (type == SELECT_QUESTION_TYPE){
        int answerNumber = questions[number].getAnswerNumber();
        for (int i=0;i<answerNumber;i++){
            QString answer = questions[number].getAnswerText(i);
            answer.remove(0,2);
            QCheckBox * checkBox = new QCheckBox(answer, this);
            guiObjects.push_back(checkBox);
            checkBox->setGeometry(600,100+i*50,500,25);
            QFont f;
            f.setPixelSize(14);
            checkBox->setFont(f);
            checkBox->show();
        }
        int i;  //отобразить ранее введенный ответ
        foreach (i, questions[number].getSelectedAnswers()) {
            qobject_cast<QCheckBox *>(guiObjects[i])->setChecked(true);
        }
    }
    if (type == INPUT_QUESTION_TYPE){
        QLineEdit * edit = new QLineEdit(this);
        guiObjects.push_back(edit);
        edit->setGeometry(600,10,400,20);
        edit->show();
        if( ! questions[currentQuestion].getAnswerText().isEmpty()){  //отобразить ранее введенный ответ
            qobject_cast<QLineEdit *>(guiObjects.first())->setText(questions[currentQuestion].getAnswerText());
        }
    }
    if (type == SEQUENCE_QUESTION_TYPE){
        showSequenceQuestion(questions[currentQuestion].getAnswersSequence());
    }
    if (type == MATCH_QUESTION_TYPE){
        int answerNumber = questions[number].getAnswerNumber();
        for (int i=0;i<answerNumber;i++){
            QString answer = questions[number].getAnswerText(i);
            answer.remove(0,2);
            QLabel * label = new QLabel(answer, this);
            guiObjects.push_back(label);
            label->setGeometry(i*(WINDOW_WIDTH/answerNumber)+20,30,500,25);
            QFont f;
            f.setPixelSize(14);
            label->setFont(f);
            label->show();
        }
        showMatchQuestion(questions[currentQuestion].getAnswersSequence());
    }
}

void TestWindow::hideQuestion() //сохранение ответов и скрытие вопроса
{
     questions[currentQuestion].resetAnswer(); //сбросить сохраненный ранее ответ
    if ( ! questions[currentQuestion].getAdvancedData().isEmpty()){
        ui->imageLabel->clear();
        ui->imageLabel->hide();
    }
    if (questions[currentQuestion].type == SELECT_QUESTION_TYPE){
        for (int i =0 ;i<guiObjects.size(); i++) {
            QCheckBox * ptr = qobject_cast<QCheckBox*> (guiObjects[i]);
            if(ptr->isChecked()){
                questions[currentQuestion].addAnswer(i);
            }
        }
    }
    if (questions[currentQuestion].type == INPUT_QUESTION_TYPE){
        QLineEdit * ptr = qobject_cast<QLineEdit*>( guiObjects.first());
        questions[currentQuestion].addAnswer(ptr->text());
    }
    if (questions[currentQuestion].type == SEQUENCE_QUESTION_TYPE){
        questions[currentQuestion].addAnswer( drawer->getSequence());
        delete scene;
        delete drawer;
    }
    if (questions[currentQuestion].type == MATCH_QUESTION_TYPE){
        questions[currentQuestion].addAnswer( gDrawer->getSequence());
        delete scene;
        delete gDrawer;
    }
    QObject* i;
    foreach (i, guiObjects) {
        delete i;
    }
    guiObjects.clear();
}

void TestWindow::showSequenceQuestion(QVector<int> restoreSequence)
{
    scene = new QGraphicsScene(this);   // Инициализируем графическую сцену
    scene->setItemIndexMethod(QGraphicsScene::NoIndex); // настраиваем индексацию элементов
    QGraphicsView * graphicsView = new QGraphicsView(this);
    guiObjects.push_back(graphicsView);
    graphicsView->resize(500,600);  // Устанавливаем размер graphicsView
    graphicsView->setGeometry(500,10,500,600);
    graphicsView->setScene(scene);  // Устанавливаем графическую сцену в graphicsView
    graphicsView->setRenderHint(QPainter::Antialiasing);    // Настраиваем рендер
    graphicsView->setCacheMode(QGraphicsView::CacheBackground); // Кэш фона
    graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    graphicsView->show();
    scene->setSceneRect(0,0,450,550); // Устанавливаем размер сцены
    drawer = new SequenceQuestionDrawer(questions[currentQuestion].getAdvancedData(),scene,restoreSequence);
    //guiObjects.push_back(reinterpret_cast<QObject*>(drawer));

}

void TestWindow::showMatchQuestion(QVector<int> restoreSequence)
{
    scene = new QGraphicsScene(this);   // Инициализируем графическую сцену
    scene->setItemIndexMethod(QGraphicsScene::NoIndex); // настраиваем индексацию элементов
    QGraphicsView * graphicsView = new QGraphicsView(this);
    guiObjects.push_back(graphicsView);
    graphicsView->resize(400,600);  // Устанавливаем размер graphicsView
    graphicsView->setGeometry(0,60,1010,470);
    graphicsView->setScene(scene);  // Устанавливаем графическую сцену в graphicsView
    graphicsView->setRenderHint(QPainter::Antialiasing);    // Настраиваем рендер
    graphicsView->setCacheMode(QGraphicsView::CacheBackground); // Кэш фона
    graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    graphicsView->show();
    scene->setSceneRect(0,0,1000,460); // Устанавливаем размер сцены
    gDrawer = new GroupQuestionDrawer(questions[currentQuestion].getAdvancedData(),scene,restoreSequence);
    //guiObjects.push_back(reinterpret_cast<QObject*>(drawer));
}
