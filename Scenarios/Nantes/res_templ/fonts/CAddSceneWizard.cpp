#include "CAddSceneWizard.h"

// Include QT
#include <QBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QScrollArea>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QDebug>
#include <QMessageBox>

CAddSceneWizard::CAddSceneWizard(int a_iActivePlayer, const std::vector<std::string>& a_rSceneIDP1,
                                 const std::vector<std::string>& a_rSceneIDP2,QWidget* a_pParent,
                                 QString a_sPreviousID1, QString a_sPreviousID2):
    QDialog(a_pParent),
    m_sPreviousID1(a_sPreviousID1),
    m_sPreviousID2(a_sPreviousID2),
    m_iTemplateNumber(1),
    m_rSceneIDP1(a_rSceneIDP1),
    m_rSceneIDP2(a_rSceneIDP2),
    m_iActivePlayer(a_iActivePlayer)
{
    QHBoxLayout* hWizardLayout = new QHBoxLayout();

    QWidget* templateWidget = new QWidget();
    QScrollArea* templateScrollArea = new QScrollArea();
    templateScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    templateScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    templateScrollArea->setStyleSheet("QLabel{ border : 1px solid white}");
    QVBoxLayout* vTemplateLayout = new QVBoxLayout();
    QPushButton* template1 = new QPushButton("template1");
    template1->setMinimumHeight(120);
    QPushButton* template2 = new QPushButton("template2");
    template2->setMinimumHeight(120);
    QPushButton* template3 = new QPushButton("template3");
    template3->setMinimumHeight(120);
    QPushButton* template4 = new QPushButton("template4");
    template4->setMinimumHeight(120);
    QPushButton* template5 = new QPushButton("template5");
    template5->setMinimumHeight(120);
    QPushButton* template6 = new QPushButton("template6");
    template6->setMinimumHeight(120);
    QVBoxLayout* vTemplateScrollAreaLayout = new QVBoxLayout();
    vTemplateLayout->addWidget(template1);
    vTemplateLayout->addWidget(template2);
    vTemplateLayout->addWidget(template3);
    vTemplateLayout->addWidget(template4);
    vTemplateLayout->addWidget(template5);
    vTemplateLayout->addWidget(template6);
    templateScrollArea->setLayout(vTemplateScrollAreaLayout);
    templateWidget->setLayout(vTemplateLayout);
    templateWidget->setMinimumWidth(300);
    templateScrollArea->setWidget(templateWidget);

    // Create preview title
    m_pPreviewTitle = new QLabel("");
    m_pPreviewTitle->setAlignment(Qt::AlignCenter);

    // Create preview widget
    QWidget* previewAndOptionWidget = new QWidget();
    QVBoxLayout* vPreviewOptionLayout = new QVBoxLayout();
    QWidget* previewWidget = new QWidget();
    previewWidget->setMinimumHeight(250);
    previewWidget->setMinimumWidth(250);
    previewWidget->setStyleSheet("QWidget{ background-color : white;}");

    // Create previous screen comboBox
    QWidget* comboBoxWidget = new QWidget();
    QHBoxLayout* hComboBoxLayout = new QHBoxLayout();
    QLabel* idComboBoxTitle = new QLabel("écran précédent : ");
    m_pComboBoxID = new QComboBox();
    if(m_sPreviousID1 != Q_NULLPTR && a_iActivePlayer == 1)
    {
        FillComboBox(a_iActivePlayer, m_sPreviousID1);
    }
    else if(m_sPreviousID2 != Q_NULLPTR && a_iActivePlayer == 2)
    {
        FillComboBox(a_iActivePlayer, m_sPreviousID2);
    }
    else
    {
        m_pComboBoxID->addItem("fin");
        m_pComboBoxID->setCurrentIndex(0);
        m_pComboBoxID->setEnabled(false);
    }
    m_pComboBoxID->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    hComboBoxLayout->addWidget(idComboBoxTitle);
    hComboBoxLayout->addWidget(m_pComboBoxID);
    comboBoxWidget->setLayout(hComboBoxLayout);


    // Create otpion widget
    QWidget* optionWidget = new QWidget();
    QHBoxLayout* hOptionLayout = new QHBoxLayout();
    QWidget* rightPart = new QWidget();
    QVBoxLayout* vRightPartLayout = new QVBoxLayout();
    QWidget* playerWidget = new QWidget();
    QHBoxLayout* hPlayerLayout = new QHBoxLayout();
    m_pPlayer1CheckBox = new QCheckBox("Joueur 1");
    m_pPlayer2CheckBox = new QCheckBox("Joueur 2");
    hPlayerLayout->addWidget(m_pPlayer1CheckBox);
    hPlayerLayout->addWidget(m_pPlayer2CheckBox);
    // initialise check box
    if(a_iActivePlayer == 2)
    {
        m_pPlayer2CheckBox->setChecked(true);
        m_pPlayer1CheckBox->setChecked(false);
    }
    else if(a_iActivePlayer == 1)
    {
        m_pPlayer1CheckBox->setChecked(true);
        m_pPlayer2CheckBox->setChecked(false);
    }
    else
    {
        m_pPlayer1CheckBox->setChecked(true);
        m_pPlayer2CheckBox->setChecked(true);
    }
    playerWidget->setLayout(hPlayerLayout);
    QWidget* synchroWidget = new QWidget();
    QHBoxLayout* hSynchroLayout = new QHBoxLayout();
    QCheckBox* synchroCheckBox = new QCheckBox("Synchronisée");
    hSynchroLayout->addWidget(synchroCheckBox);
    synchroWidget->setLayout(hSynchroLayout);
    QWidget* dashWidget = new QWidget();
    QHBoxLayout* hDashLayout = new QHBoxLayout();
    QCheckBox* dashCheckBox = new QCheckBox("Déclenche le dashboard");
    hDashLayout->addWidget(dashCheckBox);
    dashWidget->setLayout(hDashLayout);
    vRightPartLayout->addWidget(playerWidget);
    vRightPartLayout->addWidget(synchroWidget);
    vRightPartLayout->addWidget(dashWidget);
    rightPart->setLayout(vRightPartLayout);
    QWidget* leftPart = new QWidget();
    QVBoxLayout* vLeftPartLayout = new QVBoxLayout();
    QWidget* idWidget = new QWidget();
    QHBoxLayout* hIdLayout = new QHBoxLayout();
    QLabel* idTitle = new QLabel("id :");
    m_pNewID = new QLineEdit();
    m_pNewID->setPlaceholderText("scene id");
    hIdLayout->addWidget(idTitle);
    hIdLayout->addWidget(m_pNewID);
    idWidget->setLayout(hIdLayout);
    QWidget* backgroundWidget = new QWidget();
    QHBoxLayout* hBackgroundLayout = new QHBoxLayout();
    QLabel* backgroundTitle = new QLabel("Background :");
    QLineEdit* backgroundValue = new QLineEdit();
    backgroundValue->setPlaceholderText("Background image");
    hBackgroundLayout->addWidget(backgroundTitle);
    hBackgroundLayout->addWidget(backgroundValue);
    backgroundWidget->setLayout(hBackgroundLayout);
    QWidget* soundWidget = new QWidget();
    QHBoxLayout* hSoundLayout = new QHBoxLayout();
    QLabel* soundTitle = new QLabel("Fond sonore :");
    QLineEdit* soundValue = new QLineEdit();
    soundValue->setPlaceholderText("Piste Audio");
    hSoundLayout->addWidget(soundTitle);
    hSoundLayout->addWidget(soundValue);
    soundWidget->setLayout(hSoundLayout);
    vLeftPartLayout->addWidget(idWidget);
    vLeftPartLayout->addWidget(backgroundWidget);
    vLeftPartLayout->addWidget(soundWidget);
    leftPart->setLayout(vLeftPartLayout);
    hOptionLayout->addWidget(leftPart);
    hOptionLayout->addWidget(rightPart);
    optionWidget->setLayout(hOptionLayout);

    // Create button widget
    QWidget* buttonsWidget = new QWidget();
    QHBoxLayout* hButtonsLayout = new QHBoxLayout();
    QPushButton* backButton = new QPushButton("Retour");
    QPushButton* validateButton = new QPushButton("Valide");
    hButtonsLayout->addWidget(backButton);
    hButtonsLayout->addWidget(validateButton);
    buttonsWidget->setLayout(hButtonsLayout);

    // Fill Preview and option widget
    vPreviewOptionLayout->addWidget(m_pPreviewTitle);
    vPreviewOptionLayout->addWidget(previewWidget);
    vPreviewOptionLayout->addWidget(comboBoxWidget);
    vPreviewOptionLayout->addWidget(optionWidget);
    vPreviewOptionLayout->addWidget(buttonsWidget);
    previewAndOptionWidget->setLayout(vPreviewOptionLayout);

    hWizardLayout->addWidget(templateScrollArea);
    hWizardLayout->addWidget(previewAndOptionWidget);
    this->setLayout(hWizardLayout);


    // Connect pushButton
    connect(backButton, SIGNAL(clicked(bool)), this, SLOT(clickOnBack(bool)));
    connect(validateButton, SIGNAL(clicked(bool)), this, SLOT(clickOnValidate(bool)));
    connect(template1, SIGNAL(clicked(bool)), this, SLOT(setTemplate1()));
    connect(template2, SIGNAL(clicked(bool)), this, SLOT(setTemplate2()));
    connect(template3, SIGNAL(clicked(bool)), this, SLOT(setTemplate3()));
    connect(m_pPlayer1CheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeActivePlayer()));
    connect(m_pPlayer2CheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeActivePlayer()));
}

// SLOTS

void CAddSceneWizard::clickOnBack(bool)
{
    this->close();
}

void CAddSceneWizard::clickOnValidate(bool)
{
    if(m_pNewID->text() == "")
    {
        QMessageBox* pEmptyId = new QMessageBox();
        pEmptyId->setText("Veuillez attribuer un identifiant au nouvel l'écran");
        pEmptyId->addButton("Ok", QMessageBox::AcceptRole);
        pEmptyId->show();
        pEmptyId->setModal(true);
        return;
    }
    if(!m_pPlayer1CheckBox->isChecked() && !m_pPlayer2CheckBox->isChecked())
    {
        QMessageBox* pEmptyId = new QMessageBox();
        pEmptyId->setText("Choisissez au moins un joueur");
        pEmptyId->addButton("Ok", QMessageBox::AcceptRole);
        pEmptyId->show();
        pEmptyId->setModal(true);
        return;
    }
    else
    {
        // Establish player id return
        int idReturn;
        if(m_pPlayer1CheckBox->isChecked())
        {
            if(m_pPlayer2CheckBox->isChecked())
            {
                idReturn = 0; // P1 & P2
            }
            else
            {
                idReturn = 1; // Only P1
            }
        }
        else
        {
            idReturn = 2; // Only P2
        }

        QString previousID = m_pComboBoxID->currentText();

        if(idReturn == 0) // Add scene at the end of both timeline
        {
            emit validate("", m_pNewID->text(), idReturn, m_iTemplateNumber);
        }
        else if(idReturn == 2) // Add scene on player 2 timeline
        {
            // should be change and use the combo box after
            emit validate(previousID, m_pNewID->text(), idReturn, m_iTemplateNumber);
        }
        else // Add scene in player 1 timeline
        {
            // should be change and use the combo box after
            emit validate(previousID, m_pNewID->text(), idReturn, m_iTemplateNumber);
        }

        this->close();
    }
}

void CAddSceneWizard::setTemplate1()
{
    m_pPreviewTitle->setText("Template 1");
    m_iTemplateNumber = 1;
}

void CAddSceneWizard::setTemplate2()
{
    m_pPreviewTitle->setText("Template 2");
    m_iTemplateNumber = 2;
}

void CAddSceneWizard::setTemplate3()
{
    m_pPreviewTitle->setText("Template 3");
    m_iTemplateNumber = 3;
}

void CAddSceneWizard::changeActivePlayer()
{
    QString dummy("");
    if(m_pPlayer1CheckBox->isChecked())
    {
       if(m_pPlayer2CheckBox->isChecked())
       {
           // Both player -> only end available
           this->FillComboBox(0, dummy);
       }
       else
       {
           // Only P1 scene id available
           this->FillComboBox(1, m_sPreviousID1);
       }
    }
    else if(m_pPlayer2CheckBox->isChecked())
    {
        // Only P2 scene available
        this->FillComboBox(2, m_sPreviousID2);
    }
    else // Both unchecked -> only end available
    {
        this->FillComboBox(0, dummy);
    }
}

void CAddSceneWizard::FillComboBox(int a_iPlayerID, const QString& a_rActiveScene)
{
    int index = 0;
    m_pComboBoxID->clear();
    bool activeSceneFound = false;
    switch(a_iPlayerID)
    {
    case 0: // Both player
        m_pComboBoxID->addItem("fin");
        m_pComboBoxID->setEnabled(false);
        break;
    case 1:
        for(std::string id : m_rSceneIDP1)
        {
            if(id.empty()) // ignore empty id
            {
                continue;
            }
            m_pComboBoxID->addItem(QString(id.c_str()));
            // Test if active scene id is found in player id list
            // If active screen is empty jump this step
            if(QString(id.c_str()) == a_rActiveScene)
            {
                m_pComboBoxID->setCurrentIndex(index);
                activeSceneFound = true;
            }
            index++;
        }
        m_pComboBoxID->setEnabled(true);
        break;
    case 2:
        for(std::string id : m_rSceneIDP2)
        {
            if(id.empty()) // Ignore empty id
            {
                continue;
            }
            m_pComboBoxID->addItem(QString(id.c_str()));

            // Test if active scene id is found in player id list
            // If active screen is empty jump this step
            if(QString(id.c_str()) == a_rActiveScene)
            {
                m_pComboBoxID->setCurrentIndex(index);
                activeSceneFound = true;
            }
            index++;
        }
        m_pComboBoxID->setEnabled(true);
        break;
    }
    if(!activeSceneFound)
    {
        m_pComboBoxID->setCurrentIndex(0);
    }
}
