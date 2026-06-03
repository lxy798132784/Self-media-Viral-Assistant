#ifndef MEDIA_VIRAL_ASSISTANT_UI_QML_BRIDGE_H
#define MEDIA_VIRAL_ASSISTANT_UI_QML_BRIDGE_H

#include <QObject>
#include <QVariant>
#include <QFuture>
#include <memory>

namespace mva::ui {

/**
 * @brief QML 桥接类，连接 C++ 后端与 QML 前端
 *        QML bridge class connecting C++ backend with QML frontend
 *
 * @details 提供前后端数据交换、事件传递、异步调用等功能。
 *          所有 QML 可调用的方法都通过此类暴露。
 *          Provides data exchange, event passing, async calls between frontend and backend.
 *          All QML-callable methods are exposed through this class.
 */
class QmlBridge : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool initialized READ isInitialized NOTIFY initializedChanged)
    Q_PROPERTY(QString currentView READ currentView WRITE setCurrentView NOTIFY currentViewChanged)

public:
    explicit QmlBridge(QObject* parent = nullptr);
    ~QmlBridge() override;

    bool initialize();
    bool isInitialized() const;

    QString currentView() const;
    void setCurrentView(const QString& view);

    Q_INVOKABLE void fetchViralContent(const QString& platform, int limit = 50);
    Q_INVOKABLE void analyzeArticle(const QString& url);
    Q_INVOKABLE void generateSuggestions(const QString& context);
    
    Q_INVOKABLE QVariantMap getDashboardData();
    Q_INVOKABLE QVariantList getContentLibrary(int page = 1, int pageSize = 20);
    Q_INVOKABLE QVariantMap getAnalysisReport(const QString& articleId);

    Q_INVOKABLE void switchAiProvider(const QString& providerId);
    Q_INVOKABLE QVariantList getAvailableAiProviders();

    Q_INVOKABLE void saveSettings(const QVariantMap& settings);
    Q_INVOKABLE QVariantMap loadSettings();

signals:
    void initializedChanged();
    void currentViewChanged();
    
    void viralContentFetched(const QVariantList& content);
    void analysisComplete(const QVariantMap& result);
    void suggestionsGenerated(const QVariantList& suggestions);
    
    void errorOccurred(const QString& message);
    void loadingStarted(const QString& task);
    void loadingFinished(const QString& task);

private:
    struct Impl;
    std::unique_ptr<Impl> d_;
};

} // namespace mva::ui

#endif
