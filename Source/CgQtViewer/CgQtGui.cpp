

#include "CgQtGLRenderWidget.h"
#include "CgQtGui.h"
#include "CgQtMainApplication.h"
#include "../CgBase/CgEnums.h"
#include "../CgEvents/CgMouseEvent.h"
#include "../CgEvents/CgKeyEvent.h"
#include "../CgEvents/CgWindowResizeEvent.h"
#include "../CgEvents/CgLoadObjFileEvent.h"
#include "../CgEvents/CgTrackballEvent.h"
#include "../CgEvents/CgColorChangeEvent.h"         //change Color
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QPushButton>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QGroupBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QMenuBar>
#include <QActionGroup>
#include <QFileDialog>
#include <iostream>



CgQtGui::CgQtGui(CgQtMainApplication *mw)
    : m_mainWindow(mw)
{
    m_glRenderWidget = new CgQtGLRenderWidget;


    connect(m_glRenderWidget, SIGNAL(mouseEvent(QMouseEvent*)), this, SLOT(mouseEvent(QMouseEvent*)));
    connect(m_glRenderWidget, SIGNAL(viewportChanged(int,int)), this, SLOT(viewportChanged(int,int)));
    connect(m_glRenderWidget, SIGNAL(trackballChanged()), this, SLOT(slotTrackballChanged()));


    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *container = new QHBoxLayout;


    QWidget *opt_color = new QWidget;
    OptionPanelColorChange(opt_color);

    QWidget *otheropt = new QWidget;
    createOptionPaneLaneRiesenfeld_UA(otheropt);

    QWidget *otheropt2 = new QWidget;
    createOptionPanelExample(otheropt2);


    QTabWidget* m_tabs = new QTabWidget();
    m_tabs->addTab(opt_color,"&Color");        //tab name
    m_tabs->addTab(otheropt,"&L-R UA");
    m_tabs->addTab(otheropt2,"&Tab");
    container->addWidget(m_tabs);

    m_tabs->setMaximumWidth(400);

    container->addWidget(m_glRenderWidget);

    QWidget *w = new QWidget;
    w->setLayout(container);
    mainLayout->addWidget(w);

    setLayout(mainLayout);
    setWindowTitle(tr("Übung Computergrafik 1 -  SoSe 2022"));      //windows name dont work


    /* create Menu Bar */
    m_menuBar = new QMenuBar;
    QMenu *file_menu = new QMenu("&File" );
    file_menu->addAction("&Open Mesh Model", this, SLOT(slotLoadMeshFile()));
    // todo: Add Quit-Action
    m_menuBar->addMenu( file_menu );
    QMenu *settings_menu = new QMenu("&Setting" );
    QMenu *polygon_mode_menu = new QMenu("&Polygon Mode" );

    QAction* m_custom_rot=settings_menu->addAction("&Custom Rotations", m_glRenderWidget, SLOT(slotCustomRotation()));
    m_custom_rot->setCheckable(true);
    m_custom_rot->setChecked(false);

    QAction* m_lighting=settings_menu->addAction("&Lighting on", m_glRenderWidget, SLOT(slotLighting()));
    m_lighting->setCheckable(true);
    m_lighting->setChecked(false);


    QActionGroup* polygonmode_group = new QActionGroup(this);
    polygonmode_group->setExclusive(true);

    QAction* points=polygon_mode_menu->addAction("&Points", m_glRenderWidget, SLOT(slotPolygonPoints()));
    points->setCheckable(true);
    points->setChecked(false);


    QAction* wireframe=polygon_mode_menu->addAction("&Wireframe", m_glRenderWidget, SLOT(slotPolygonWireframe()));
    wireframe->setCheckable(true);
    wireframe->setChecked(true);

    QAction* filled=polygon_mode_menu->addAction("&Filled", m_glRenderWidget, SLOT(slotPolygonFilled()));
    filled->setCheckable(true);
    filled->setChecked(false);


    polygonmode_group->addAction(points);
    polygonmode_group->addAction(wireframe);
    polygonmode_group->addAction(filled);


    // todo: Add Quit-Action
    m_menuBar->addMenu( file_menu );
    m_menuBar->addMenu( settings_menu );
    m_menuBar->addMenu( polygon_mode_menu );

    m_mainWindow->setMenuBar(m_menuBar);

}

QSlider *CgQtGui::createSlider()
{
    QSlider *slider = new QSlider(Qt::Vertical);
    slider->setRange(0, 360 * 16);
    slider->setSingleStep(16);
    slider->setPageStep(15 * 16);
    slider->setTickInterval(15 * 16);
    slider->setTickPosition(QSlider::TicksRight);
    return slider;
}

void CgQtGui::OptionPanelColorChange(QWidget* parent)
{
    QVBoxLayout *tab_ColorChange = new QVBoxLayout();

    /*Example for using a label */
    QLabel *options_label = new QLabel("Farbe auswählen in RGB");
    tab_ColorChange->addWidget(options_label);
    options_label->setAlignment(Qt::AlignCenter);


    /*Spinboxes for RGB Color Change  */
    //mySpinBox1->setSuffix("   suffix");
    SpinBoxRed = new QSpinBox();
    tab_ColorChange->addWidget(SpinBoxRed);
    SpinBoxRed->setMinimum(0);
    SpinBoxRed->setMaximum(255);
    SpinBoxRed->setValue(0);
    SpinBoxRed->setPrefix("R: ");

    SpinBoxGreen = new QSpinBox();
    tab_ColorChange->addWidget(SpinBoxGreen);
    SpinBoxGreen->setMinimum(0);
    SpinBoxGreen->setMaximum(255);
    SpinBoxGreen->setValue(255);
    SpinBoxGreen->setPrefix("G: ");

    SpinBoxBlue = new QSpinBox();
    tab_ColorChange->addWidget(SpinBoxBlue);
    SpinBoxBlue->setMinimum(0);
    SpinBoxBlue->setMaximum(255);
    SpinBoxBlue->setValue(0);
    SpinBoxBlue->setPrefix("B: ");

    //change value by chaning the value of the spinbox
    connect(SpinBoxRed, SIGNAL(valueChanged(int)), this, SLOT(slotButtonChangeColorPressed()));
    connect(SpinBoxGreen, SIGNAL(valueChanged(int)), this, SLOT(slotButtonChangeColorPressed()));
    connect(SpinBoxBlue, SIGNAL(valueChanged(int)), this, SLOT(slotButtonChangeColorPressed()));

    tab_ColorChange->addWidget(SpinBoxRed);
    tab_ColorChange->addWidget(SpinBoxGreen);
    tab_ColorChange->addWidget(SpinBoxBlue);

    /*Button for RBG Color change */
    QPushButton* ButtonChangeColor = new QPushButton("Farbe bestätigen");
    tab_ColorChange->addWidget(ButtonChangeColor);

    /*  Signale:
    Signale sind öffentlich zugängliche Funktionen und können von überall ausgegeben werden,
    aber wir empfehlen, sie nur von der Klasse zu senden, die das Signal und seine
    Unterklassen definiert.
    Wenn ein Signal ausgegeben wird, werden die damit verbundenen Slots normalerweise
    sofort ausgeführt, genau wie ein normaler Funktionsaufruf.
    Wenn mehrere Slots mit einem Signal verbunden sind, werden die Slots nacheinander in der
    Reihenfolge ausgeführt, in der sie verbunden wurden, wenn das Signal ausgegeben wird.
    Signale werden vom moc automatisch generiert und müssen nicht in die .cpp-Datei
    implementiert werden. Sie können niemals Rückgabetypen haben (d. h. void verwenden).
        Schlüssel:
    Ein Slot wird aufgerufen, wenn ein damit verbundenes Signal ausgegeben wird.
    Slots sind normale C++-Funktionen und können normal aufgerufen werden; ihre einzige
    Besonderheit ist, dass Signale an sie angeschlossen werden können.
    Sie können Slots auch als virtuell definieren, was wir in der Praxis als sehr nützlich
    empfunden haben.
    */
    //use function pointers
    connect(ButtonChangeColor, SIGNAL( clicked() ), this, SLOT(slotButtonChangeColorPressed()));

    parent->setLayout(tab_ColorChange);
}

void CgQtGui::createOptionPaneLaneRiesenfeld_UA(QWidget* parent)
{
    QVBoxLayout *tab_LR_UA = new QVBoxLayout();

    /*Example for using a label */
    QLabel *options_label = new QLabel("Lane-Riesenfeld Unterteilungsalgorithmus");
    tab_LR_UA->addWidget(options_label);
    options_label->setAlignment(Qt::AlignCenter);

    /*Spinboxes for the Algorithm */
    SpinBox_LR_UA = new QSpinBox();
    tab_LR_UA->addWidget(SpinBox_LR_UA);
    SpinBox_LR_UA->setMinimum(0);
    SpinBox_LR_UA->setMaximum(10);
    SpinBox_LR_UA->setValue(0);
    SpinBox_LR_UA->setPrefix("Unterteilungsschritte: ");
    tab_LR_UA->addWidget(SpinBox_LR_UA);
//    connect(SpinBox_LR_UA, SIGNAL( valueChanged(int)  ), this, SLOT(fnk()));

    //set spacing
    tab_LR_UA->addSpacing(50);


    /*checkbox for the normals */
    CheckBox_shownormals = new QCheckBox("show normals");
    CheckBox_shownormals->setCheckable(true);
    CheckBox_shownormals->setChecked(false);
    tab_LR_UA->addWidget(CheckBox_shownormals);
//    connect(CheckBox_shownormals, SIGNAL( clicked() ), this, SLOT(slotMyCheckBox1Changed()) );


    /*Button for RBG Color change */
    QPushButton* Button_LR_UA = new QPushButton("click");
    tab_LR_UA->addWidget(Button_LR_UA);
//    connect(Button_LR_UA, SIGNAL( clicked() ), this, SLOT(slotButtonChangeColorPressed()));

    QPushButton* Button_LR_UA_reset = new QPushButton("reset");
    tab_LR_UA->addWidget(Button_LR_UA_reset);
//    connect(Button_LR_UA_reset, SIGNAL( clicked() ), this, SLOT(slotButtonChangeColorPressed()));

    parent->setLayout(tab_LR_UA);


}

void CgQtGui::createOptionPanelExample(QWidget* parent)
{


    /*Example for using a checkbox */

//    myCheckBox1 = new QCheckBox("enable Option 1");
//    myCheckBox1->setCheckable(true);
//    myCheckBox1->setChecked(false);
//    connect(myCheckBox1, SIGNAL( clicked() ), this, SLOT(slotMyCheckBox1Changed()) );
//    tab1_control->addWidget(myCheckBox1);


    QVBoxLayout *tab_control = new QVBoxLayout();
    QHBoxLayout *subBox = new QHBoxLayout();



    /*Example for using a button group */

    QGroupBox* myGroupBox = new QGroupBox("Radiobutton Group Example ");

    myButtonGroup = new QButtonGroup(subBox);
    myButtonGroup->setExclusive(true);

    QRadioButton* radiobutton1 = new QRadioButton( "&Option1");
    QRadioButton* radiobutton2 = new QRadioButton( "&Option2");
    QRadioButton* radiobutton3 = new QRadioButton( "&Option3");
    QRadioButton* radiobutton4 = new QRadioButton( "&Option4");
    QRadioButton* radiobutton5 = new QRadioButton( "&Option5");

    radiobutton2->setChecked(true);

    myButtonGroup->addButton(radiobutton1,0);
    myButtonGroup->addButton(radiobutton2,1);
    myButtonGroup->addButton(radiobutton3,2);
    myButtonGroup->addButton(radiobutton4,3);
    myButtonGroup->addButton(radiobutton5,4);


    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(radiobutton1);
    vbox->addWidget(radiobutton2);
    vbox->addWidget(radiobutton3);
    vbox->addWidget(radiobutton4);
    vbox->addWidget(radiobutton5);
    vbox->addStretch(1);
    myGroupBox->setLayout(vbox);
    subBox->addWidget(myGroupBox);
    tab_control->addLayout(subBox);

    connect(myButtonGroup, SIGNAL( buttonClicked(int) ), this, SLOT( slotButtonGroupSelectionChanged() ) );
    parent->setLayout(tab_control);

}



void CgQtGui::slotButtonGroupSelectionChanged()
{

}

void CgQtGui::slotMySpinBox1Changed()
{

}

void CgQtGui::slotMyCheckBox1Changed()
{

}


void CgQtGui::slotLoadMeshFile()
{



   QString file=  QFileDialog::getOpenFileName(this, tr("Open Obj-File"),"",tr("Model Files (*.obj)"));


    CgBaseEvent* e = new CgLoadObjFileEvent(Cg::LoadObjFileEvent, file.toStdString());

    notifyObserver(e);
}


void CgQtGui::slotTrackballChanged()
{
    CgBaseEvent* e = new CgTrackballEvent(Cg::CgTrackballEvent, m_glRenderWidget->getTrackballRotation());
    notifyObserver(e);
}

void CgQtGui::slotButtonChangeColorPressed()
{
   std::cout << "button 1 pressed to change the color" << std::endl;
   CgBaseEvent* e= new CgColorChangeEvent(Cg::CgButtonColorChangePress, SpinBoxRed->value(), SpinBoxGreen->value(), SpinBoxBlue->value());
   notifyObserver(e);

}


void CgQtGui::mouseEvent(QMouseEvent* event)
{

   // std::cout << QApplication::keyboardModifiers() << std::endl;

  //  if(QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)==true)
    //    std::cout << Cg::ControlModifier << endl;


   if(event->type()==QEvent::MouseButtonPress)
   {


        CgBaseEvent* e = new CgMouseEvent(Cg::CgMouseButtonPress,
                                          glm::vec2(event->localPos().x() ,event->localPos().y()),
                                          (Cg::MouseButtons)event->button());

        notifyObserver(e);
   }

   if(event->type()==QEvent::MouseMove)
   {
       CgBaseEvent* e= new CgMouseEvent(Cg::CgMouseMove,
                                        glm::vec2(event->localPos().x() ,event->localPos().y()),
                                        (Cg::MouseButtons)event->button());
       notifyObserver(e);
   }



}

void CgQtGui::keyPressEvent(QKeyEvent *event)
{
   CgBaseEvent* e= new CgKeyEvent(Cg::CgKeyPressEvent,(Cg::Key)event->key(),(Cg::KeyboardModifiers)event->nativeModifiers(),event->text().toStdString());
   notifyObserver(e);
}


void CgQtGui::viewportChanged(int w, int h)
{
     CgBaseEvent* e = new CgWindowResizeEvent(Cg::WindowResizeEvent,w,h);
     notifyObserver(e);
}




CgBaseRenderer* CgQtGui::getRenderer()
{
    return m_glRenderWidget;
}





