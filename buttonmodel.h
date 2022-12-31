#ifndef BUTTONMODEL_H
#define BUTTONMODEL_H
#include <QAbstractItemModel>
#include <QDebug>
#include <QString>
#include <qpoint.h>

class Button {

public:
    Button(QString name)
    {
        m_name = name;
    }

    Button(QString name, QPoint point)
    {
        m_name = name;
        m_position = point;
    }

    void setText(QString name)
    {
        m_name = name;
    }

    void setPosition(const QPoint& point)
    {
        m_position = point;
    }

    void setChecked(bool checked)
    {
        m_checked = checked;
    }

    void setVisible(bool visible)
    {
        m_visible = visible;
    }

    bool m_visible = true;
    bool m_checked = false;
    bool m_pressed = false;
    QString m_name;
    QPoint m_position;
};

class ButtonModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum ButtonRoles {
        NameRole = Qt::UserRole + 1,
        PositionRole,
        SizeRole,
        CheckedRole,
        VisibleRole,
        PressedRole,
    };

    int rowCount(const QModelIndex& parent = QModelIndex()) const override
    {
        Q_UNUSED(parent);
        return _buttons.count();
    }
    int columnCount(const QModelIndex& parent) const override
    {
        Q_UNUSED(parent);
        return 1;
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const
    {
        // qDebug() << "DATA" << index;
        if (index.row() < 0 || index.row() >= _buttons.count())
            return QVariant();

        Button* button = _buttons[index.row()];
        if (role == NameRole) {
            return button->m_name;
        } else if (role == PositionRole)
            return button->m_position;
        else if (role == CheckedRole) {
            return button->m_checked;
        } else if (role == VisibleRole) {
            return button->m_visible;
        }
        return QVariant();
    }

    void addButton(Button* button)
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());

        _buttons << button;
        endInsertRows();
    }

    void setText(int i, QString name)
    {
        if (i >= 0 && i < _buttons.size()) {
            _buttons[i]->setText(name);
        }
        setData(index(i, 0), name, NameRole);
        //Q_EMIT dataChanged(index(0, 0), index(rowCount(), 1), QVector<int>() << NameRole);
        // beginResetModel();
        //endResetModel();
    }

    void setPosition(int i, QPoint position)
    {
        if (i >= 0 && i < _buttons.size()) {
            _buttons[i]->setPosition(position);
        }
        setData(index(i, 0), position, PositionRole);
        //Q_EMIT dataChanged(index(0, 0), index(rowCount(), 1), QVector<int>() << PositionRole);
        // beginResetModel();
        // endResetModel();
    }

    void setChecked(int i, bool checked)
    {

        if (i >= 0 && i < _buttons.size()) {
            _buttons[i]->setChecked(checked);
        }
        setData(index(i, 0), checked, CheckedRole);
        //Q_EMIT dataChanged(index(0, 0), index(rowCount(), 1), QVector<int>() << CheckedRole);
        // beginResetModel();
        // endResetModel();
    }
    void setVisible(int i, bool visible)
    {

        if (i >= 0 && i < _buttons.size()) {
            _buttons[i]->setVisible(visible);
        }
        setData(index(i, 0), visible, VisibleRole);
        // Q_EMIT dataChanged(index(0, 0), index(rowCount(), 1), QVector<int>() << VisibleRole);
        // beginResetModel();
        // endResetModel();
    }

    QHash<int, QByteArray> roleNames() const
    {
        QHash<int, QByteArray> roles;
        roles[NameRole] = "name";
        roles[PositionRole] = "pos";
        roles[SizeRole] = "size";
        roles[CheckedRole] = "check";
        roles[VisibleRole] = "visible";
        roles[PressedRole] = "pressed";
        return roles;
    }
    bool setData(const QModelIndex& index, const QVariant& value, int role)
    {

        if (index.isValid() && role == PressedRole) {

            if (value.toBool() == true) {
                Q_EMIT sigPressed(index.row());
            }

            if (value.toBool() == false) {
                Q_EMIT sigReleased(index.row());
            }

            return true;
        }
        Q_EMIT dataChanged(index, index, QVector<int>() << role);
        return false;
    }

    QVector<Button*> buttons()
    {
        return _buttons;
    }

private:
    QVector<Button*> _buttons;

    // QAbstractItemModel interface
public:
    Qt::ItemFlags flags(const QModelIndex& index)
    {

        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    }

Q_SIGNALS:
    void sigPressed(int index);
    void sigReleased(int index);
};

#endif // BUTTONMODEL_H
