/**
 * @file main.qml
 * @brief 应用程序主界面
 *        Main Application UI
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 1440
    height: 900
    minimumWidth: 1024
    minimumHeight: 768
    
    title: qsTr("自媒体爆款助手 / Media Viral Assistant")
    
    // Color scheme / 配色方案
    readonly property color primaryColor: "#3498db"
    readonly property color secondaryColor: "#2ecc71"
    readonly property color backgroundColor: "#f5f6fa"
    readonly property color textColor: "#2c3e50"
    
    header: ToolBar {
        Material.background: primaryColor
        
        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 20
            
            Label {
                text: qsTr("🎯 自媒体爆款助手")
                font.pixelSize: 20
                font.bold: true
                color: "white"
            }
            
            Item { Layout.fillWidth: true }
            
            // Platform selector / 平台选择器
            ComboBox {
                id: platformSelector
                ModelRole: "name"
                model: ListModel {
                    ListElement { name: "All Platforms / 全部平台"; code: "all" }
                    ListElement { name: "WeChat / 微信"; code: "wechat" }
                    ListElement { name: "Douyin / 抖音"; code: "douyin" }
                    ListElement { name: "Xiaohongshu / 小红书"; code: "xiaohongshu" }
                    ListElement { name: "Bilibili / B 站"; code: "bilibili" }
                    ListElement { name: "Weibo / 微博"; code: "weibo" }
                }
                
                Component.onCompleted: currentIndex = 0
                
                delegate: ItemDelegate {
                    width: parent.width
                    contentItem: Text {
                        text: model.name
                        color: platformSelector.currentIndex === index ? primaryColor : textColor
                    }
                }
            }
            
            // Refresh button / 刷新按钮
            ToolButton {
                icon.source: "qrc:/icons/refresh.svg"
                tooltip: qsTr("Refresh / 刷新")
                onClicked: loadViralContent()
            }
            
            // Settings button / 设置按钮
            ToolButton {
                icon.source: "qrc:/icons/settings.svg"
                tooltip: qsTr("Settings / 设置")
                onClicked: settingsDrawer.open()
            }
        }
    }
    
    // Sidebar navigation / 侧边栏导航
    Pane {
        id: sidebar
        width: 220
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        Material.background: "#ffffff"
        Material.elevation: 2
        
        ColumnLayout {
            anchors.fill: parent
            spacing: 5
            
            Repeater {
                model: ListModel {
                    ListElement { 
                        icon: "📊"
                        name: "Dashboard / 仪表盘"
                        page: "dashboard"
                    }
                    ListElement { 
                        icon: "📚"
                        name: "Content Library / 内容库"
                        page: "library"
                    }
                    ListElement { 
                        icon: "🔍"
                        name: "Analysis Report / 拆解报告"
                        page: "analysis"
                    }
                    ListElement { 
                        icon: "💡"
                        name: "Topic Ideas / 选题推荐"
                        page: "topics"
                    }
                    ListElement { 
                        icon: "⚙️"
                        name: "Settings / 设置"
                        page: "settings"
                    }
                }
                
                delegate: ItemDelegate {
                    width: sidebar.width - 20
                    height: 50
                    Layout.leftMargin: 10
                    
                    background: Rectangle {
                        color: navStack.currentNav === model.page ? 
                               Qt.lighter(primaryColor, 1.1) : "transparent"
                        radius: 8
                    }
                    
                    contentItem: RowLayout {
                        spacing: 10
                        
                        Text {
                            text: model.icon
                            font.pixelSize: 20
                        }
                        
                        Text {
                            text: model.name
                            font.pixelSize: 14
                            color: navStack.currentNav === model.page ? 
                                   primaryColor : textColor
                            elide: Text.ElideRight
                        }
                    }
                    
                    onClicked: {
                        navStack.currentNav = model.page
                        stackLayout.currentIndex = index
                    }
                }
            }
        }
    }
    
    // Main content area / 主内容区域
    StackLayout {
        id: stackLayout
        anchors.left: sidebar.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        
        // Dashboard Page / 仪表盘页面
        DashboardPage {
            id: dashboardPage
        }
        
        // Content Library Page / 内容库页面
        ContentLibraryPage {
            id: libraryPage
        }
        
        // Analysis Report Page / 拆解报告页面
        AnalysisReportPage {
            id: analysisPage
        }
        
        // Topic Recommendation Page / 选题推荐页面
        TopicRecommendationPage {
            id: topicPage
        }
        
        // Settings Page / 设置页面
        SettingsPage {
            id: settingsPage
        }
    }
    
    // Settings Drawer / 设置抽屉
    Drawer {
        id: settingsDrawer
        width: Math.min(mainWindow.width * 0.8, 500)
        height: mainWindow.height
        parent: mainWindow
        
        ColumnLayout {
            anchors.fill: parent
            spacing: 20
            
            Label {
                text: qsTr("Settings / 设置")
                font.pixelSize: 24
                font.bold: true
            }
            
            GroupBox {
                title: qsTr("Data Sources / 数据源")
                Layout.fillWidth: true
                
                ColumnLayout {
                    anchors.fill: parent
                    
                    CheckBox {
                        text: "Jizhile Data / 极致了数据"
                        checked: true
                    }
                    
                    CheckBox {
                        text: "Xinbang / 新榜"
                        checked: true
                    }
                    
                    CheckBox {
                        text: "Xigua Data / 西瓜数据"
                        checked: false
                    }
                }
            }
            
            GroupBox {
                title: qsTr("AI Provider / AI 提供商")
                Layout.fillWidth: true
                
                ColumnLayout {
                    anchors.fill: parent
                    
                    RadioButton {
                        text: "OpenAI GPT-4"
                        checked: true
                    }
                    
                    RadioButton {
                        text: "Anthropic Claude"
                    }
                    
                    RadioButton {
                        text: "Local LLM / 本地模型"
                    }
                }
            }
            
            Button {
                text: qsTr("Save & Close / 保存并关闭")
                Layout.alignment: Qt.AlignRight
                onClicked: settingsDrawer.close()
            }
        }
    }
    
    // Navigation state / 导航状态
    property string currentNav: "dashboard"
    
    // Functions / 函数
    function loadViralContent() {
        console.log("Loading viral content for platform:", 
                   platformSelector.currentText)
        // Implementation would call C++ backend
    }
    
    Component.onCompleted: {
        console.log("Application initialized / 应用程序已初始化")
        loadViralContent()
    }
}
