#ifndef OBJECT_VIEW_H
#define OBJECT_VIEW_H


#include <QWidget>
#include "object_scene.h"

namespace Ui {
class Object_Widget;
}

/*!
 * \class Object_Widget
 * \brief Klasa Object_Widget która zawiera w sobie widget odpowiedzialny za wyświetlenie sceny 3D
 */
class Object_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Object_Widget(QWidget *parent = nullptr);

    ~Object_Widget();

    /*!
     * \brief Funkcja zwraca wskaźnik do QWidgetu stworzonego w formularzu "object_widget.ui".
     * \return Wskaźnik na scene.
     */
    Object_Scene *renderScene();
public slots:


private:
    Ui::Object_Widget *ui;
};

#endif // OBJECT_VIEW_H
