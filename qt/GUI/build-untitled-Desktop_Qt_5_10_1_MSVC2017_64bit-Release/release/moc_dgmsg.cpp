/****************************************************************************
** Meta object code from reading C++ file 'dgmsg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../untitled/dgmsg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dgmsg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DGMsg_t {
    QByteArrayData data[23];
    char stringdata0[201];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DGMsg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DGMsg_t qt_meta_stringdata_DGMsg = {
    {
QT_MOC_LITERAL(0, 0, 5), // "DGMsg"
QT_MOC_LITERAL(1, 6, 11), // "msgReceived"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 14), // "unsigned char*"
QT_MOC_LITERAL(4, 34, 7), // "rawdata"
QT_MOC_LITERAL(5, 42, 7), // "sendout"
QT_MOC_LITERAL(6, 50, 7), // "quint8*"
QT_MOC_LITERAL(7, 58, 5), // "data1"
QT_MOC_LITERAL(8, 64, 8), // "quint16*"
QT_MOC_LITERAL(9, 73, 5), // "data2"
QT_MOC_LITERAL(10, 79, 8), // "starthid"
QT_MOC_LITERAL(11, 88, 7), // "startRH"
QT_MOC_LITERAL(12, 96, 7), // "stophid"
QT_MOC_LITERAL(13, 104, 6), // "stopRH"
QT_MOC_LITERAL(14, 111, 11), // "isConnected"
QT_MOC_LITERAL(15, 123, 6), // "update"
QT_MOC_LITERAL(16, 130, 16), // "QAbstractSeries*"
QT_MOC_LITERAL(17, 147, 6), // "series"
QT_MOC_LITERAL(18, 154, 3), // "num"
QT_MOC_LITERAL(19, 158, 10), // "startCalib"
QT_MOC_LITERAL(20, 169, 9), // "getRawMsg"
QT_MOC_LITERAL(21, 179, 10), // "processMsg"
QT_MOC_LITERAL(22, 190, 10) // "packetType"

    },
    "DGMsg\0msgReceived\0\0unsigned char*\0"
    "rawdata\0sendout\0quint8*\0data1\0quint16*\0"
    "data2\0starthid\0startRH\0stophid\0stopRH\0"
    "isConnected\0update\0QAbstractSeries*\0"
    "series\0num\0startCalib\0getRawMsg\0"
    "processMsg\0packetType"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DGMsg[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       5,    2,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   77,    2, 0x0a /* Public */,
      11,    0,   78,    2, 0x0a /* Public */,
      12,    0,   79,    2, 0x0a /* Public */,
      13,    0,   80,    2, 0x0a /* Public */,
      14,    0,   81,    2, 0x0a /* Public */,
      15,    2,   82,    2, 0x0a /* Public */,
      19,    0,   87,    2, 0x0a /* Public */,
      20,    0,   88,    2, 0x0a /* Public */,
      21,    1,   89,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 8,    7,    9,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void, 0x80000000 | 16, QMetaType::Int,   17,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 22,    4,

       0        // eod
};

void DGMsg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DGMsg *_t = static_cast<DGMsg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->msgReceived((*reinterpret_cast< unsigned char*(*)>(_a[1]))); break;
        case 1: _t->sendout((*reinterpret_cast< quint8*(*)>(_a[1])),(*reinterpret_cast< quint16*(*)>(_a[2]))); break;
        case 2: { bool _r = _t->starthid();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->startRH(); break;
        case 4: _t->stophid(); break;
        case 5: _t->stopRH(); break;
        case 6: { bool _r = _t->isConnected();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->update((*reinterpret_cast< QAbstractSeries*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->startCalib(); break;
        case 9: _t->getRawMsg(); break;
        case 10: _t->processMsg((*reinterpret_cast< packetType(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSeries* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (DGMsg::*_t)(unsigned char * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DGMsg::msgReceived)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (DGMsg::*_t)(quint8 * , quint16 * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DGMsg::sendout)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DGMsg::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DGMsg.data,
      qt_meta_data_DGMsg,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DGMsg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DGMsg::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DGMsg.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DGMsg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void DGMsg::msgReceived(unsigned char * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DGMsg::sendout(quint8 * _t1, quint16 * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_DGMsgThread_t {
    QByteArrayData data[6];
    char stringdata0[65];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DGMsgThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DGMsgThread_t qt_meta_stringdata_DGMsgThread = {
    {
QT_MOC_LITERAL(0, 0, 11), // "DGMsgThread"
QT_MOC_LITERAL(1, 12, 12), // "updateSeries"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 15), // "hiddisconnected"
QT_MOC_LITERAL(4, 42, 16), // "calibStepChanged"
QT_MOC_LITERAL(5, 59, 5) // "index"

    },
    "DGMsgThread\0updateSeries\0\0hiddisconnected\0"
    "calibStepChanged\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DGMsgThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,
       3,    0,   30,    2, 0x06 /* Public */,
       4,    1,   31,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,

       0        // eod
};

void DGMsgThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DGMsgThread *_t = static_cast<DGMsgThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateSeries(); break;
        case 1: _t->hiddisconnected(); break;
        case 2: _t->calibStepChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (DGMsgThread::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DGMsgThread::updateSeries)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (DGMsgThread::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DGMsgThread::hiddisconnected)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (DGMsgThread::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DGMsgThread::calibStepChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DGMsgThread::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DGMsgThread.data,
      qt_meta_data_DGMsgThread,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DGMsgThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DGMsgThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DGMsgThread.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DGMsgThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void DGMsgThread::updateSeries()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void DGMsgThread::hiddisconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void DGMsgThread::calibStepChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
