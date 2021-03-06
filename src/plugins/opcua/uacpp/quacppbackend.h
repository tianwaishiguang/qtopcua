/******************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtOpcUa module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#ifndef QUACPPASYNCBACKEND_H
#define QUACPPASYNCBACKEND_H

#include <private/qopcuabackend_p.h>

#include <QtCore/QMutex>
#include <QtCore/QSet>
#include <QtCore/QString>
#include <QtCore/QTimer>

#include <uabase.h>
#include <uaclientsdk.h>

QT_BEGIN_NAMESPACE

class QUACppClient;
class QUACppNode;
class QUACppSubscription;

class UACppAsyncBackend : public QOpcUaBackend,
        public UaClientSdk::UaSessionCallback
{
    Q_OBJECT
public:
    UACppAsyncBackend(QUACppClient *parent);
    ~UACppAsyncBackend();

    void connectionStatusChanged(OpcUa_UInt32 clientConnectionId, UaClientSdk::UaClient::ServerStatus serverStatus) override;

public Q_SLOTS:
    void connectToEndpoint(const QUrl &url);
    void disconnectFromEndpoint();

    void browse(quint64 handle, const UaNodeId &id, const QOpcUaBrowseRequest &request);
    void readAttributes(quint64 handle, const UaNodeId &id, QOpcUa::NodeAttributes attr, QString indexRange);
    void writeAttribute(quint64 handle, const UaNodeId &id, QOpcUa::NodeAttribute attrId, QVariant value, QOpcUa::Types type, QString indexRange);
    void writeAttributes(quint64 handle, const UaNodeId &id, QOpcUaNode::AttributeMap toWrite, QOpcUa::Types valueAttributeType);
    void enableMonitoring(quint64 handle, const UaNodeId &id, QOpcUa::NodeAttributes attr, const QOpcUaMonitoringParameters &settings);
    void modifyMonitoring(quint64 handle, QOpcUa::NodeAttribute attr, QOpcUaMonitoringParameters::Parameter item, QVariant value);
    void disableMonitoring(quint64 handle, QOpcUa::NodeAttributes attr);
    void callMethod(quint64 handle, const UaNodeId &objectId, const UaNodeId &methodId, QVector<QOpcUa::TypedVariant> args);
    void resolveBrowsePath(quint64 handle, const UaNodeId &startNode, const QVector<QOpcUa::QRelativePathElement> &path);
    void requestEndpoints(const QUrl &url);

    bool removeSubscription(quint32 subscriptionId);

public:
    QUACppSubscription *getSubscription(const QOpcUaMonitoringParameters &settings);
    QUACppSubscription *getSubscriptionForItem(quint64 handle, QOpcUa::NodeAttribute attr);
    void cleanupSubscriptions();
    Q_DISABLE_COPY(UACppAsyncBackend);
    UaClientSdk::UaSession *m_nativeSession;
    QUACppClient *m_clientImpl;
    QHash<quint32, QUACppSubscription *> m_subscriptions;
    QHash<quint64, QHash<QOpcUa::NodeAttribute, QUACppSubscription *>> m_attributeMapping; // Handle -> Attribute -> Subscription
    static quint32 m_numClients;
    static bool m_platformLayerInitialized;
    QMutex m_lifecycleMutex;
    double m_minPublishingInterval;
    bool m_disableEncryptedPasswordCheck{false};
};

QT_END_NAMESPACE

#endif // QUACPPASYNCBACKEND_H
