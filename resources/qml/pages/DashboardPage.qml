import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {
    id: dashboardPage
    title: qsTr("仪表盘 / Dashboard")
    
    ScrollView {
        anchors.fill: parent
        
        ColumnLayout {
            width: dashboardPage.width - 40
            spacing: 20
            
            // Summary Cards Row
            RowLayout {
                Layout.fillWidth: true
                spacing: 16
                
                // Total Viral Content Card
                Pane {
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 120
                    
                    ColumnLayout {
                        anchors.fill: parent
                        
                        Label {
                            text: "🔥"
                            font.pixelSize: 32
                        }
                        
                        Label {
                            text: qsTr("爆款内容")
                            font.pixelSize: 14
                            color: "gray"
                        }
                        
                        Label {
                            text: "1,234"
                            font.pixelSize: 28
                            font.bold: true
                            color: Material.color(Material.Red, Material.Shade700)
                        }
                        
                        Label {
                            text: qsTr("+12% 较上周")
                            font.pixelSize: 12
                            color: "green"
                        }
                    }
                }
                
                // Total Analysis Card
                Pane {
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 120
                    
                    ColumnLayout {
                        anchors.fill: parent
                        
                        Label {
                            text: "📊"
                            font.pixelSize: 32
                        }
                        
                        Label {
                            text: qsTr("分析报告")
                            font.pixelSize: 14
                            color: "gray"
                        }
                        
                        Label {
                            text: "567"
                            font.pixelSize: 28
                            font.bold: true
                            color: Material.color(Material.Blue, Material.Shade700)
                        }
                        
                        Label {
                            text: qsTr("+8% 较上周")
                            font.pixelSize: 12
                            color: "green"
                        }
                    }
                }
                
                // AI Suggestions Card
                Pane {
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 120
                    
                    ColumnLayout {
                        anchors.fill: parent
                        
                        Label {
                            text: "💡"
                            font.pixelSize: 32
                        }
                        
                        Label {
                            text: qsTr("AI 建议")
                            font.pixelSize: 14
                            color: "gray"
                        }
                        
                        Label {
                            text: "89"
                            font.pixelSize: 28
                            font.bold: true
                            color: Material.color(Material.Purple, Material.Shade700)
                        }
                        
                        Label {
                            text: qsTr("待处理")
                            font.pixelSize: 12
                            color: "orange"
                        }
                    }
                }
                
                // Platforms Monitored Card
                Pane {
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 120
                    
                    ColumnLayout {
                        anchors.fill: parent
                        
                        Label {
                            text: "🌐"
                            font.pixelSize: 32
                        }
                        
                        Label {
                            text: qsTr("监控平台")
                            font.pixelSize: 14
                            color: "gray"
                        }
                        
                        Label {
                            text: "7"
                            font.pixelSize: 28
                            font.bold: true
                            color: Material.color(Material.Teal, Material.Shade700)
                        }
                        
                        Label {
                            text: qsTr("全部在线")
                            font.pixelSize: 12
                            color: "green"
                        }
                    }
                }
            }
            
            // Chart Area
            Pane {
                Layout.fillWidth: true
                Layout.preferredHeight: 300
                
                ColumnLayout {
                    anchors.fill: parent
                    
                    Label {
                        text: qsTr("爆款趋势 / Viral Trend")
                        font.pixelSize: 18
                        font.bold: true
                    }
                    
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: Material.color(Material.Gray, Material.Shade50)
                        radius: 8
                        
                        Label {
                            anchors.centerIn: parent
                            text: qsTr("[图表区域 - 待实现]\n[Chart Area - To be implemented]")
                            horizontalAlignment: Text.AlignHCenter
                            color: "gray"
                        }
                    }
                }
            }
            
            // Recent Activity
            Pane {
                Layout.fillWidth: true
                Layout.preferredHeight: 250
                
                ColumnLayout {
                    anchors.fill: parent
                    
                    Label {
                        text: qsTr("最近活动 / Recent Activity")
                        font.pixelSize: 18
                        font.bold: true
                    }
                    
                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        
                        model: ListModel {
                            ListElement { time: "10:30"; action: "分析完成：微信公众号文章"; platform: "微信" }
                            ListElement { time: "10:15"; action: "发现爆款：抖音短视频"; platform: "抖音" }
                            ListElement { time: "09:45"; action: "生成选题建议 5 条"; platform: "AI" }
                            ListElement { time: "09:30"; action: "数据同步完成"; platform: "系统" }
                            ListElement { time: "09:00"; action: "开始每日监控任务"; platform: "系统" }
                        }
                        
                        delegate: RowLayout {
                            width: ListView.view.width
                            spacing: 12
                            
                            Label {
                                text: model.time
                                width: 50
                                color: "gray"
                            }
                            
                            Label {
                                text: model.action
                                Layout.fillWidth: true
                            }
                            
                            Badge {
                                text: model.platform
                            }
                        }
                    }
                }
            }
        }
    }
}
