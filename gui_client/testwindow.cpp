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
    showSequenceQuestion();//test
}

void TestWindow::showQuestion(int number)
{

    if ( ! questions[currentQuestion].getAdvancedData().isEmpty()){ //показать изображение если есть
        QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData( questions[currentQuestion].getAdvancedData() );
            ui->imageLabel->setPixmap(outPixmap);
            ui->imageLabel->setScaledContents( true );
            ui->imageLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
            ui->imageLabel->show();
    }
    int type = questions[number].type;
    ui->label->setText(questions[number].getQuestionText());
    if (type == SELECT_QUESTION_TYPE){
        int answerNumber = questions[number].getAnswerNumber();
        for (int i=0;i<answerNumber;i++){
            QCheckBox * checkBox = new QCheckBox(questions[number].getAnswerText(i), this);
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
        edit->setGeometry(600,100,400,15);
        edit->show();
        if( ! questions[currentQuestion].getAnswerText().isEmpty()){  //отобразить ранее введенный ответ
            qobject_cast<QCheckBox *>(guiObjects.first())->setText(questions[currentQuestion].getAnswerText());
        }
    }
    if (type == SEQUENCE_QUESTION_TYPE){
        showSequenceQuestion();
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
    }
    QObject* i;
    foreach (i, guiObjects) {
        delete i;
    }
    guiObjects.clear();
}

void TestWindow::showSequenceQuestion()
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
    drawer = new SequenceQuestionDrawer(questions[currentQuestion].getAdvancedData(),scene);
    //guiObjects.push_back(reinterpret_cast<QObject*>(drawer));

}
