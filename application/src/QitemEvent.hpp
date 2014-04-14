#ifndef EVENTITEM_HPP
#define EVENTITEM_HPP

#include <QTreeWidgetItem>

#ifndef INC_VIDEO
#define INC_VIDEO
#include "Event/Event.hpp"
#endif

#ifndef INC_VIDEO
#define INC_VIDEO
#include "Video/Video.hpp"
#endif

#define TAG_EVENT "Event"

class EventItem : public QTreeWidgetItem
{
public:
    // Constructors
    explicit EventItem(QTreeWidget *parent = 0);
    EventItem(QString eventname, QTreeWidget *parent = 0);

    // Destructors
    ~EventItem();

    // Functions
    EventPtr getEvent();
    void setEvent(EventPtr inevent);
private:
    EventPtr event;
};

#endif // EVENTITEM_HPP
