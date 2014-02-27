#include "QtreeVideos.hpp"

QtreeVideos::QtreeVideos(QWidget *parent) :
    QTreeWidget(parent)
{
    nEvent = 0;

    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this, SLOT(on_item_doubleclicked(QTreeWidgetItem*,int)));
    connect(this, SIGNAL(itemSelectionChanged()),
            this, SLOT(on_item_selection_changed()));
}

void QtreeVideos::on_add_video_file(){
    VideoItem * last = NULL;
    QFileDialog getFileDialog(this);
    getFileDialog.setDirectory(QDir::homePath());
    getFileDialog.setFileMode(QFileDialog::ExistingFiles);
    getFileDialog.setAcceptMode(QFileDialog::AcceptOpen);

    QStringList fileNames;
    if(getFileDialog.exec())
    {
        fileNames = getFileDialog.selectedFiles();
        if (fileNames.isEmpty()) {
            showMessage(tr("No files loaded"));
            return;
        } // No file name provided
        foreach (QString fileName, fileNames) {
            last = new VideoItem(fileName);
            addTopLevelItem(last);
            showMessage(tr("Loaded ") + fileName);
        }
        if(last != NULL) {
            setPlaybackEnabled(false); //FIXME: SIGNAL SLOT
            loadVid(last->getVideo(), PLAYER_VID); //FIXME: SIGNAL SLOT
            showMessage(tr("Finished loading files"));
        }
    } else {
        showMessage(tr("No files selected"));
    }
    return;
}

void QtreeVideos::on_auto_detect_events(){
    VideoItem* videoiter;
    std::deque<Event*> events;
    EventItem* newEvent;

    unsigned int j;

    SplitDialog split;

    if (selectedItems().isEmpty()){
        showMessage(tr("Select at least one Video"));
        return;
    }

    foreach (QTreeWidgetItem* item, selectedItems()){
        if (item->parent() != NULL){
            showMessage(tr("Item selected is not a video"));
            continue;
        }
        videoiter = (VideoItem*) item;
        showMessage(tr("Analyzing Video %1").arg(item->text(0)));
        split.setFPS(videoiter->getVideo()->getFPS());
        split.setWindowTitle(videoiter->text(0));
        if(!split.exec()){
            showMessage(tr("Auto detection canceled"));
            continue;
        }

        // Progress bar signals and slots
        connect(videoiter->getVideo(), SIGNAL(startProgress(uint, uint)),
                this, SIGNAL(startProgress(uint,uint)));
        connect(videoiter->getVideo(), SIGNAL(progressChanged(uint)),
                this, SIGNAL(progressChanged(uint)));

        events = videoiter->getVideo()->autoDetectEvents(split.getThreshold(),
                                                         split.getMaxFrames(),
                                                         split.getMinFrames(),
                                                         split.getHistory(),
                                                         split.getvarThreshold(),
                                                         split.getbShadowDetection(),
                                                         (getSettings()->getCacheDir().append("/")).toStdString());

        resetProgress();
        for (j = 0; j < events.size(); j++) {
            showMessage(tr("Found Event %1").arg(nEvent));
            newEvent = new EventItem(QString("E%1").arg(nEvent));;
            newEvent->setEvent(events.at(j));
            nEvent ++;
            videoiter->addChild(newEvent);
        }
        if (j != 0){
            videoiter->setExpanded(true);
        }
    }
}

void QtreeVideos::on_remove_from_project()
{
    if(isVisible()){
        foreach(QTreeWidgetItem* item, selectedItems()){
            if(item->parent() == NULL){
                delete item;
            }
            else {
                showMessage(tr("Item selected is not a Video"));
            }
        }
    }
}

void QtreeVideos::on_delete_event()
{
    if(isVisible()){
        foreach(QTreeWidgetItem* item, selectedItems()){
            if(item->parent() != NULL){
                EventItem* evItem = (EventItem *) item;
                removePlayer((Player *) evItem->getEvent());

                delete item;
            }
            else {
                showMessage(tr("Item selected is not an Event"));
            }
        }
    }
}

void QtreeVideos::on_item_doubleclicked(QTreeWidgetItem *item, int column){
    showMessage(tr("Loaded ") + item->text(0));
    if (item->parent() == NULL){
        VideoItem * vItem = (VideoItem *) item;
        setPlaybackEnabled(false);
        loadVid(vItem->getVideo(), PLAYER_VID);
    } else {
        EventItem * vItem = (EventItem *) item;
        Event* ev = vItem->getEvent();
        setPlaybackMode(getPlayMode());
        setPlaybackEnabled(true);
        loadVid(ev, PLAYER_EV);
    }
    playOrPause();
}

void QtreeVideos::on_item_selection_changed()
{
    EventItem* eventIt;
    int selected = 0;
    double frames = 0;
    double time = 0;
    foreach (QTreeWidgetItem* item, selectedItems()){
        eventIt = (EventItem *) item;
        frames += eventIt->getEvent()->getLengthFrames();
        time += eventIt->getEvent()->getLengthTime();
        selected ++;
    }
    setSelectedText(QString("%1").arg(selected));
    setTotalFramesText(QString("%1").arg(frames));
    setTotalTimeText(QString("%1 s").arg(time));
}

void QtreeVideos::flip_horizontally(){
    VideoItem * vidItem;
    foreach (QTreeWidgetItem* item, selectedItems()){
        if(item->parent() != NULL){
            showMessage(tr("Flip Videos, not events."));
            continue;
        }
        vidItem = (VideoItem*) item;
        vidItem->getVideo()->flip_horizontally();
        showMessage(tr("Flipped selected Video."));
    }
}

void QtreeVideos::flip_vertically(){
    VideoItem * vidItem;
    foreach (QTreeWidgetItem* item, selectedItems()){
        if(item->parent() != NULL){
            showMessage(tr("Flip Videos, not events."));
            continue;
        }
        vidItem = (VideoItem*) item;
        vidItem->getVideo()->flip_vertically();
        showMessage(tr("Flipped selected Video."));
    }
}

void QtreeVideos::on_calibrate()
{
    if (selectedItems().size() == 0){
        showMessage(tr("Select a Video first"));
        return;
    }

    CalibrationDialog calibDiag;
    if(!calibDiag.exec()){
        showMessage(tr("Calibration canceled"));
        return;
    }

    VideoItem* videoIt;
    foreach (QTreeWidgetItem* item, selectedItems()){
        if (item->parent() != NULL){
            continue;
        }
        videoIt = (VideoItem*) item;
        videoIt->getVideo()->calibrate(calibDiag.getNBoards(),
                                       calibDiag.getFrameStep(),
                                       calibDiag.getWidth(),
                                       calibDiag.getHeight(),
                                       calibDiag.getIterations());
        if(videoIt->getVideo()->isCalibrated()){
            showMessage(tr("Video Calibrated"));
            playOrPause();
        } else {
            showMessage(tr("Failed Calibration"));
        }
    }
}

void QtreeVideos::on_import_camera(){
    VideoItem * vidItem;
    int i = 0;

    foreach (QTreeWidgetItem* item, selectedItems()){
        if(item->parent() != NULL){
            showMessage(tr("Export camera only implemented for Videos, not Events"));
            continue;
        }
        vidItem = (VideoItem*) item;
        vidItem->getVideo()->importCamera();

        i++;
    }
    if (!i){
        showMessage(tr("Select at least one video first"));
    }
}

void QtreeVideos::on_export_camera(){
    VideoItem * vidItem;
    foreach (QTreeWidgetItem* item, selectedItems()){
        if(item->parent() != NULL){
            showMessage(tr("Export camera only implemented for Videos, not Events"));
            continue;
        }
        vidItem = (VideoItem*) item;
        if(vidItem->getVideo()->getCamera()->write_file()){
            showMessage(tr("Camera saved to file"));
        } else {
            showMessage(tr("Failed to save camera to file"));
        }
    }
}

void QtreeVideos::on_merge()
{
    unsigned int count = 0;
    EventItem* it0, * it1;
    foreach (QTreeWidgetItem* item, selectedItems()){
        if(item->parent() == NULL){
            showMessage(tr("Merging Videos is not supported"));
            return;
        }
        else{
            switch (count){
                case 0:
                    it0 = (EventItem*) item;
                    break;
                case 1:
                    it1 = (EventItem*) item;
                    break;
                case 2:
                    showMessage(tr("Events can only be merged in pairs"));
                    return;
            }
            count++;
        }
    }
    if (count == 0 || count == 1){
        showMessage(tr("Select two events to be merged"));
        return;
    }
    showMessage(tr("Merging events %1 and %2").arg(it0->text(0), it1->text(0)));
    *(it0->getEvent()) += *(it1->getEvent());
    showMessage(tr("Merged events %1 and %2 into event %1").arg(it0->text(0), it1->text(0)));

    delete(it1);
}
