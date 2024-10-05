/****************************************************************************
** Meta object code from reading C++ file 'CQFieldRunners.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "CQFieldRunners.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQFieldRunners.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CQFieldRunnersApp_t {
    QByteArrayData data[3];
    char stringdata0[29];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CQFieldRunnersApp_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CQFieldRunnersApp_t qt_meta_stringdata_CQFieldRunnersApp = {
    {
QT_MOC_LITERAL(0, 0, 17), // "CQFieldRunnersApp"
QT_MOC_LITERAL(1, 18, 9), // "timerSlot"
QT_MOC_LITERAL(2, 28, 0) // ""

    },
    "CQFieldRunnersApp\0timerSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQFieldRunnersApp[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void CQFieldRunnersApp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CQFieldRunnersApp *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->timerSlot(); break;
        default: ;
        }
    }
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject CQFieldRunnersApp::staticMetaObject = { {
    QMetaObject::SuperData::link<QFrame::staticMetaObject>(),
    qt_meta_stringdata_CQFieldRunnersApp.data,
    qt_meta_data_CQFieldRunnersApp,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CQFieldRunnersApp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQFieldRunnersApp::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CQFieldRunnersApp.stringdata0))
        return static_cast<void*>(this);
    return QFrame::qt_metacast(_clname);
}

int CQFieldRunnersApp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_CQFieldRunnersCanvas_t {
    QByteArrayData data[1];
    char stringdata0[21];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CQFieldRunnersCanvas_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CQFieldRunnersCanvas_t qt_meta_stringdata_CQFieldRunnersCanvas = {
    {
QT_MOC_LITERAL(0, 0, 20) // "CQFieldRunnersCanvas"

    },
    "CQFieldRunnersCanvas"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQFieldRunnersCanvas[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void CQFieldRunnersCanvas::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject CQFieldRunnersCanvas::staticMetaObject = { {
    QMetaObject::SuperData::link<CQCanvas2D::staticMetaObject>(),
    qt_meta_stringdata_CQFieldRunnersCanvas.data,
    qt_meta_data_CQFieldRunnersCanvas,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CQFieldRunnersCanvas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQFieldRunnersCanvas::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CQFieldRunnersCanvas.stringdata0))
        return static_cast<void*>(this);
    return CQCanvas2D::qt_metacast(_clname);
}

int CQFieldRunnersCanvas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CQCanvas2D::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
