import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {
    id: settingsPage
    title: qsTr("设置 / Settings")
    
    ScrollView {
        anchors.fill: parent
        
        ColumnLayout {
            width: settingsPage.width - 40
            spacing: 20
            
            GroupBox {
                Layout.fillWidth: true
                title: qsTr("🤖 AI 提供商 / AI Providers")
                
                ColumnLayout {
                    anchors.fill: parent
                    
                    RowLayout {
                        Layout.fillWidth: true
                        
                        Label { 
                            text: "OpenAI API Key:"
                            width: 150
                        }
                        
                        TextField {
                            Layout.fillWidth: true
                            placeholderText: "sk-..."
                            echoMode: TextInput.Password
                        }
                    }
                    
                    RowLayout {
                        Layout.fillWidth: true
                        
                        Label { 
                            text: "Claude API Key:"
                            width: 150
                        }
                        
                        TextField {
                            Layout.fillWidth: true
                            placeholderText: "sk-ant-..."
                            echoMode: TextInput.Password
                        }
                    }
                }
            }
            
            GroupBox {
                Layout.fillWidth: true
                title: qsTr("📊 数据源 / Data Sources")
                
                ColumnLayout {
                    anchors.fill: parent
                    
                    CheckBox { text: "极致了数据 API"; checked: true }
                    CheckBox { text: "新榜 API"; checked: true }
                    CheckBox { text: "西瓜数据 API"; checked: false }
                    CheckBox { text: "清博智能 API"; checked: false }
                    CheckBox { text: "WeWe RSS"; checked: true }
                }
            }
            
            GroupBox {
                Layout.fillWidth: true
                title: qsTr("🌐 平台监控 / Platform Monitoring")
                
                ColumnLayout {
                    anchors.fill: parent
                    
                    RowLayout {
                        CheckBox { text: "微信"; checked: true }
                        CheckBox { text: "抖音"; checked: true }
                        CheckBox { text: "小红书"; checked: true }
                    }
                    RowLayout {
                        CheckBox { text: "B 站"; checked: true }
                        CheckBox { text: "微博"; checked: false }
                        CheckBox { text: "快手"; checked: false }
                    }
                }
            }
            
            GroupBox {
                Layout.fillWidth: true
                title: qsTr("⚙️ 系统设置 / System Settings")
                
                ColumnLayout {
                    anchors.fill: parent
                    
                    RowLayout {
                        Label { text: qsTr("语言 / Language:"); width: 150 }
                        ComboBox {
                            model: ["简体中文", "English", "繁體中文"]
                            currentIndex: 0
                        }
                    }
                    
                    RowLayout {
                        Label { text: qsTr("主题 / Theme:"); width: 150 }
                        ComboBox {
                            model: [qsTr("浅色 / Light"), qsTr("深色 / Dark"), qsTr("自动 / Auto")]
                            currentIndex: 0
                        }
                    }
                    
                    RowLayout {
                        Label { text: qsTr("日志级别 / Log Level:"); width: 150 }
                        ComboBox {
                            model: ["DEBUG", "INFO", "WARNING", "ERROR"]
                            currentIndex: 1
                        }
                    }
                }
            }
            
            RowLayout {
                Layout.fillWidth: true
                
                Item { Layout.fillWidth: true }
                
                Button {
                    text: qsTr("重置 / Reset")
                }
                
                Button {
                    text: qsTr("保存 / Save")
                    highlighted: true
                    
                    onClicked: {
                        // Save settings
                    }
                }
            }
        }
    }
}
