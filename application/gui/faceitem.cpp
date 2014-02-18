#include "faceitem.hpp"

FaceItem::FaceItem(QTreeWidgetItem *parent) :
    QTreeWidgetItem(parent)
{

}

FaceItem::FaceItem(QString facename, Face* inface, QTreeWidget *parent) :
    QTreeWidgetItem(parent)
{
    setText(0, facename);
    std::ostringstream ss;
    ss << inface->faceNumber();
    setText(1, ss.str().c_str());

    for (unsigned int i=0; i<inface->faceNumber(); i++){
        SnapshotItem * newsnap = new SnapshotItem("FIXME",inface->getFaceAt(i));
        this->addChild(newsnap);
    }
}

FaceItem::~FaceItem(){
    delete face;
}

Face* FaceItem::getFace(){
    return face;
}

SnapshotItem::SnapshotItem(QTreeWidgetItem *parent) :
    QTreeWidgetItem(parent)
{

}

SnapshotItem::SnapshotItem(QString snapname, Snapshot *insnap, QTreeWidget *parent) :
    QTreeWidgetItem(parent)
{
    snap = insnap;
    setText(0, snapname);
}

SnapshotItem::~SnapshotItem(){

}

Snapshot* SnapshotItem::getSnapshot(){
    return snap;
}
