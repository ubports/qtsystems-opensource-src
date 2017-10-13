/****************************************************************************
**
** Copyright (C) 2016 Canonical, Ltd. and/or its subsidiary(-ies).
** Copyright (C) 2015 The Qt Company Ltd and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSystems module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QINPUTINFO_H
#define QINPUTINFO_H

#include "qsysteminfoglobal.h"
#include <QObject>
#include <QVector>
#include <QMap>
#include <QSocketNotifier>
#include <QDebug>
#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

class QInputInfoManagerPrivate;
class QInputDevicePrivate;
class QInputDevice;

class QInputInfoManager;

#ifndef QT_NO_MIR
class QInputInfoManagerMir;
#endif
#ifndef QT_NO_UDEV
class QInputInfoManagerUdev;
#endif

class Q_SYSTEMINFO_EXPORT QInputDevice : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantMap properties READ properties CONSTANT)
    Q_PROPERTY(QInputDevice::InputTypeFlags types READ types CONSTANT)

public:

    enum InputType {
        UnknownType = 0,
        Button = 1,
        Mouse = 2,
        TouchPad = 4,
        TouchScreen = 8,
        Keyboard = 16,
        Switch = 32
    };
    Q_ENUMS(InputType)

    Q_DECLARE_FLAGS(InputTypeFlags, InputType)
    Q_FLAGS(InputTypeFlags)

    explicit QInputDevice(QObject *parent = Q_NULLPTR);
    QString name() const;
    QString identifier() const;
    QList <int> buttons() const; //keys event code
    QList <int> switches() const;
    QList <int> relativeAxes() const;
    QList <int> absoluteAxes() const;
    QInputDevice::InputTypeFlags types() const;
    /*
     * name
     * identifier
     * buttons
     * switches
     * rAxes
     * xAxes
     * types
     * */
    QVariantMap properties();

private:

    QInputDevicePrivate *d_ptr;

    friend class QInputDeviceManagerPrivate;
#ifndef QT_NO_MIR
    friend class QInputInfoManagerMir;
#endif
#ifndef QT_NO_UDEV
    friend class QInputInfoManagerUdev;
#endif
    void setName(const QString &);
    void setIdentifier(const QString &);
    void addButton(int);
    void addSwitch(int);
    void addRelativeAxis(int);
    void addAbsoluteAxis(int);
    void setTypes(QInputDevice::InputTypeFlags flags);
    QVariantMap deviceProperties;

};

class Q_SYSTEMINFO_EXPORT QInputInfoManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QInputDevice::InputTypeFlags filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(QInputDevice * lastAdded READ lastAdded NOTIFY deviceAdded)

public:

    explicit QInputInfoManager(QObject *parent = Q_NULLPTR);
    ~QInputInfoManager();

    int count() const;
    int count(const QInputDevice::InputType filter) const;

    void setFilter(QInputDevice::InputTypeFlags filterFlags);
    QInputDevice::InputTypeFlags filter();

    QMap <QString, QInputDevice *> deviceMap();
    QInputDevice *lastAdded();

Q_SIGNALS:

    void ready();
    void deviceAdded(QInputDevice *inputDevice);
    void deviceRemoved(const QString &deviceId);

    void countChanged(int count);
    void filterChanged(QInputDevice::InputTypeFlags filterFlags);

private Q_SLOTS:
    void addedDevice(QInputDevice *devicePath);
    void removedDevice(const QString &deviceId);

private:

    Q_DISABLE_COPY(QInputInfoManager)
#if !defined(QT_SIMULATOR)
    QInputInfoManagerPrivate *const d_ptr;
    Q_DECLARE_PRIVATE(QInputInfoManager)
#endif
    QInputDevice::InputTypeFlags currentFilter;
    QMap <QString, QInputDevice *> currentFilteredMap;
    int filteredCount;

private Q_SLOTS:
    void privateReady();
};


QT_END_NAMESPACE

Q_DECLARE_METATYPE(QT_PREPEND_NAMESPACE(QInputDevice::InputType))
Q_DECLARE_METATYPE(QT_PREPEND_NAMESPACE(QInputDevice::InputTypeFlags))
Q_DECLARE_OPERATORS_FOR_FLAGS(QT_PREPEND_NAMESPACE(QInputDevice::InputTypeFlags))

#endif // QINPUTINFO_H
