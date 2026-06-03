import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Material 2.15

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 1400
    height: 900
    title: qsTr("自媒体爆款助手 / Media Viral Assistant")
    
    Material.theme: Material.Light
    Material.primary: Material.Blue
    Material.accent: Material.Teal

    // Header
    header: ToolBar {
        Material.elevation: 2
        
        RowLayout {
            anchors.fill: parent
            spacing: 16
            
            Label {
                text: "🔥 " + qsTr("自媒体爆款助手")
                font.pixelSize: 20
                font.bold: true
                color: Material.color(Material.Blue, Material.Shade900)
            }
            
            Item { Layout.fillWidth: true }
            
            // AI Provider Selector
            ComboBox {
                id: aiProviderSelector
                model: ["OpenAI GPT-4", "Claude 3", "通义千问", "文心一言"]
                currentIndex: 0
                
                Label {
                    text: "🤖 AI:"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.left
                    anchors.rightMargin: 8
                }
            }
            
            // User Menu
            ToolButton {
                text: "👤"
                
                onClicked: userMenu.open()
                
                Menu {
                    id: userMenu
                    MenuItem { text: qsTr("设置 / Settings") }
                    MenuItem { text: qsTr("关于 / About") }
                    MenuItem { text: qsTr("退出 / Quit") }
                }
            }
        }
    }

    // Sidebar Navigation
    Pane {
        id: sidebar
        width: 220
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        Material.elevation: 1
        
        ColumnLayout {
            anchors.fill: parent
            spacing: 4
            
            Repeater {
                model: [
                    { icon: "📊", name: qsTr("仪表盘 / Dashboard"), view: "dashboard" },
                    { icon: "📚", name: qsTr("内容库 / Content Library"), view: "library" },
                    { icon: "🔍", name: qsTr("拆解报告 / Analysis"), view: "analysis" },
                    { icon: "💡", name: qsTr("选题推荐 / Suggestions"), view: "suggestions" },
                    { icon: "🎯", name: qsTr("阿里系 / Alibaba"), view: "alibaba" },
                    { icon: "🎵", name: qsTr("字节系 / ByteDance"), view: "bytedance" },
                    { icon: "🛒", name: qsTr("PDD 系 / PDD"), view: "pdd" },
                    { icon: "📹", name: qsTr("快手系 / Kuaishou"), view: "kuaishou" },
                    { icon: "💬", name: qsTr("腾讯系 / Tencent"), view: "tencent" },
                    { icon: "🐦", name: qsTr("微博系 / Weibo"), view: "weibo" },
                    { icon: "📺", name: qsTr("B 站系 / Bilibili"), view: "bilibili" },
                    { icon: "⚙️", name: qsTr("设置 / Settings"), view: "settings" }
                ]
                
                delegate: ToolButton {
                    width: sidebar.width - 16
                    text: modelData.icon + "  " + modelData.name
                    checkable: true
                    checked: currentView === modelData.view
                    
                    onClicked: {
                        currentView = modelData.view
                        stackLayout.currentIndex = index
                    }
                    
                    background: Rectangle {
                        color: parent.checked ? Material.color(Material.Blue, Material.Shade100) : "transparent"
                        radius: 8
                    }
                }
            }
        }
    }

    // Main Content Area
    StackLayout {
        id: stackLayout
        anchors.left: sidebar.right
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        clip: true
        
        currentIndex: 0
        
        // Dashboard Page
        DashboardPage { id: dashboardPage }
        
        // Content Library Page
        ContentLibraryPage { id: libraryPage }
        
        // Analysis Page
        AnalysisPage { id: analysisPage }
        
        // Suggestions Page
        SuggestionsPage { id: suggestionsPage }
        
        // Platform Pages (placeholders)
        Page { 
            Label { 
                text: "阿里系平台 / Alibaba Platforms" 
                anchors.centerIn: parent 
            } 
        }
        Page { 
            Label { 
                text: "字节系平台 / ByteDance Platforms" 
                anchors.centerIn: parent 
            } 
        }
        Page { 
            Label { 
                text: "PDD 平台 / PDD Platform" 
                anchors.centerIn: parent 
            } 
        }
        Page { 
            Label { 
                text: "快手平台 / Kuaishou Platform" 
                anchors.centerIn: parent 
            } 
        }
        Page { 
            Label { 
                text: "腾讯平台 / Tencent Platforms" 
                anchors.centerIn: parent 
            } 
        }
        Page { 
            Label { 
                text: "微博平台 / Weibo Platform" 
                anchors.centerIn: parent 
            } 
        }
        Page { 
            Label { 
                text: "B 站平台 / Bilibili Platform" 
                anchors.centerIn: parent 
            } 
        }
        
        // Settings Page
        SettingsPage { id: settingsPage }
    }

    property string currentView: "dashboard"
}
