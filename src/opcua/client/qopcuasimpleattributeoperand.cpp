/****************************************************************************
**
** Copyright (C) 2015 basysKom GmbH, opensource@basyskom.com
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtOpcUa module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
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
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qopcuasimpleattributeoperand.h"
#include "qopcuaqualifiedname.h"

QT_BEGIN_NAMESPACE

/*!
    \class QOpcUaSimpleAttributeOperand
    \inmodule QtOpcUa
    \brief The OPC UA SimpleAttributeOperand type.

    The SimpleAttributeOperand is specified in OPC-UA part 4, 7.4.4.5.
    It is used when a node attribute is required as operand.

    For example, the following simple attribute operand represents the value
    of the "Severity" field of the base event type:
    \code
        QOpcUaSimpleAttributeOperand("Severity");
    \endcode
*/
class QOpcUaSimpleAttributeOperandData : public QSharedData
{
public:
    QString typeId{QStringLiteral("ns=0;i=2041")}; // BaseEventType
    QVector<QOpcUaQualifiedName> browsePath;
    QOpcUa::NodeAttribute attributeId {QOpcUa::NodeAttribute::Value};
    QString indexRange;
};

QOpcUaSimpleAttributeOperand::QOpcUaSimpleAttributeOperand()
    : data(new QOpcUaSimpleAttributeOperandData)
{
}

/*!
    Constructs a simple attribute operand from \a rhs.
*/
QOpcUaSimpleAttributeOperand::QOpcUaSimpleAttributeOperand(const QOpcUaSimpleAttributeOperand &rhs)
    : data(rhs.data)
{
}

/*!
    Constructs a simple attribute operand for attribute \a attributeId of the direct child with the browse name
    \a name in namespace \a namespaceIndex. \a typeId is the node id of a type definition node. The operand will
    be restricted to instances of type \a typeId or a subtype.
*/
QOpcUaSimpleAttributeOperand::QOpcUaSimpleAttributeOperand(const QString &name, quint16 namespaceIndex, const QString &typeId, QOpcUa::NodeAttribute attributeId)
    : data(new QOpcUaSimpleAttributeOperandData)
{
    browsePathRef().append(QOpcUaQualifiedName(namespaceIndex, name));
    setTypeId(typeId);
    setAttributeId(attributeId);
}

/*!
    Constructs a simple attribute operand for the attribute \a attributeId of an object or variable of type \a typeId.
    This can be used for requesting the ConditionId in an event filter as described in OPC-UA part 9, Table 8.
*/
QOpcUaSimpleAttributeOperand::QOpcUaSimpleAttributeOperand(QOpcUa::NodeAttribute attributeId, const QString &typeId)
    : data(new QOpcUaSimpleAttributeOperandData)
{
    setTypeId(typeId);
    setAttributeId(attributeId);
}

/*!
    Sets the values from \a rhs in this simple attribute operand.
*/
QOpcUaSimpleAttributeOperand &QOpcUaSimpleAttributeOperand::operator=(const QOpcUaSimpleAttributeOperand &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

/*!
    Returns \c true if this simple attribute operand has the same value as \a rhs.
*/
bool QOpcUaSimpleAttributeOperand::operator==(const QOpcUaSimpleAttributeOperand &rhs) const
{
    return attributeId() == rhs.attributeId() && browsePath() == rhs.browsePath() &&
            indexRange() == rhs.indexRange() && typeId() == rhs.typeId();
}

/*!
    Converts this simple attribute operand to \l QVariant.
*/
QOpcUaSimpleAttributeOperand::operator QVariant() const
{
    return QVariant::fromValue(*this);
}

QOpcUaSimpleAttributeOperand::~QOpcUaSimpleAttributeOperand()
{
}

/*!
    Returns the index range string.
*/
QString QOpcUaSimpleAttributeOperand::indexRange() const
{
    return data->indexRange;
}

/*!
    Sets the index range string used to identify a single value or subset of the attribute's value to \a indexRange.
*/
void QOpcUaSimpleAttributeOperand::setIndexRange(const QString &indexRange)
{
    data->indexRange = indexRange;
}

/*!
    Returns the attribute of the node \l browsePath is pointing to.
*/
QOpcUa::NodeAttribute QOpcUaSimpleAttributeOperand::attributeId() const
{
    return data->attributeId;
}

/*!
    Sets the attribute id to \a attributeId.
*/
void QOpcUaSimpleAttributeOperand::setAttributeId(QOpcUa::NodeAttribute attributeId)
{
    data->attributeId = attributeId;
}

/*!
    Returns the relative path to a node starting from \l typeId.
*/
QVector<QOpcUaQualifiedName> QOpcUaSimpleAttributeOperand::browsePath() const
{
    return data->browsePath;
}

/*!
    Returns a reference to the browse path.

    \sa browsePath()
*/
QVector<QOpcUaQualifiedName> &QOpcUaSimpleAttributeOperand::browsePathRef()
{
    return data->browsePath;
}

/*!
    Sets the browse path to the node holding the attribute to \a browsePath.
*/
void QOpcUaSimpleAttributeOperand::setBrowsePath(const QVector<QOpcUaQualifiedName> &browsePath)
{
    data->browsePath = browsePath;
}

/*!
    Returns the node id of a type definition node.
*/
QString QOpcUaSimpleAttributeOperand::typeId() const
{
    return data->typeId;
}

/*!
    Sets the node id of the type definition node to \a typeId. The operand will be of the type or one of its subtypes.
*/
void QOpcUaSimpleAttributeOperand::setTypeId(const QString &typeId)
{
    data->typeId = typeId;
}

QT_END_NAMESPACE
