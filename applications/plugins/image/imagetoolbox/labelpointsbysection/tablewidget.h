#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QtGui>
#include <QMap>

#include <sofa/defaulttype/VecTypes.h>
#include <iostream>



class TableWidgetForLabelPointBySectionToolBoxAction: public QWidgetAction
{
Q_OBJECT

public:
    typedef sofa::defaulttype::Vec3f Vec3f;

    struct Point
    {
        Vec3f ip;
        Vec3f p;
    };

    typedef sofa::helper::vector<Point> VecCoord;
    typedef QMap<unsigned int, VecCoord > MapSection;


private:

    QComboBox * listSection;
    QTableWidget * listPoints;
    QPushButton * deteteSection;
    QPushButton * deletePoint;
    QPushButton * moveupPoint;
    QPushButton * movedownPoint;
    
    MapSection * mapSection;
    unsigned int currentSection;
    
public:
    
    TableWidgetForLabelPointBySectionToolBoxAction(QObject *parent): QWidgetAction(parent)
    {
        listSection = new QComboBox();
        listPoints = new QTableWidget();
        deteteSection  = new QPushButton("del section");
        deletePoint = new QPushButton("del");
        deletePoint->setEnabled(false);
        moveupPoint = new QPushButton("up");
        moveupPoint->setEnabled(false);
        movedownPoint = new QPushButton("down");
        movedownPoint->setEnabled(false);
    
        QVBoxLayout *vlayout = new QVBoxLayout();
    
        vlayout->addWidget(new QLabel("section"));
        QHBoxLayout *hlayout = new QHBoxLayout();
        hlayout->addWidget(listSection);
        hlayout->addWidget(deteteSection);
    
        vlayout->addLayout(hlayout);
        vlayout->addWidget(listPoints);
    
        QHBoxLayout *hlayout2 = new QHBoxLayout();
        hlayout2->addWidget(deletePoint);
        hlayout2->addWidget(moveupPoint);
        hlayout2->addWidget(movedownPoint);
    
        vlayout->addLayout(hlayout2);
        
        QGroupBox *box = new QGroupBox();
        box->setLayout(vlayout);
    
        this->setDefaultWidget(box);
        
        listPoints->insertColumn(0);
        listPoints->setHorizontalHeaderItem (0, new QTableWidgetItem("X") );
        listPoints->insertColumn(1);
        listPoints->setHorizontalHeaderItem (1, new QTableWidgetItem("Y") );
        listPoints->insertColumn(2);
        listPoints->setHorizontalHeaderItem (2, new QTableWidgetItem("Z") );
        listPoints->setSelectionBehavior(QAbstractItemView::SelectRows);
        listPoints->setSelectionMode(QAbstractItemView::SingleSelection);
        listPoints->setEditTriggers(QAbstractItemView::NoEditTriggers);
        
        connect(listSection,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxchangeSection(int)));

        
        //updateData();
    }

    inline void setMapSection(MapSection *m){mapSection=m;updateData();}
    
    void updateData()
    {
        if(!mapSection)
        {
            return;
        }
        disconnect(listSection,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxchangeSection(int)));

        // lister les sections disponibles
        listSection->clear();
        QList<unsigned int> key = mapSection->keys();
        for(int i=0;i<key.size();i++)
        {
            if(currentSection!=key[i] && (mapSection->value(key[i])).size()==0)
            {
                mapSection->remove(key[i]);
            }
            else
            {
                listSection->addItem(QString::number(key[i]));
                if(currentSection==key[i])listSection->setCurrentIndex(i);
            }
        }
        
        // lister les point de la section courante
        VecCoord &vector = mapSection->operator [](currentSection);
        
        while(listPoints->rowCount()!=0)
            listPoints->removeRow(0);
        

        for(unsigned int i=0;i<vector.size();i++)
        {
            listPoints->insertRow(i);
            listPoints->setItem(i,0,new QTableWidgetItem(QString::number(vector[i].ip.x())));
            listPoints->setItem(i,1,new QTableWidgetItem(QString::number(vector[i].ip.y())));
            listPoints->setItem(i,2,new QTableWidgetItem(QString::number(vector[i].ip.z())));
            
        }

        listPoints->resizeColumnsToContents();
        connect(listSection,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxchangeSection(int)));
    }

    void setSection(unsigned int cs){currentSection=cs;}

private slots:
    void comboBoxchangeSection(int i)
    {
        currentSection = mapSection->keys()[i];
        updateData();
        emit changeSection(currentSection);
    }
    
signals:
    void changeSection(int);
    
};


#endif // TABLEWIDGET_H
