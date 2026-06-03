import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {
    id: libraryPage
    title: qsTr("内容库 / Content Library")
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 16
        
        // Search and Filter Bar
        RowLayout {
            Layout.fillWidth: true
            spacing: 12
            
            TextField {
                id: searchField
                Layout.fillWidth: true
                placeholderText: qsTr("搜索内容... / Search content...")
                
                onAccepted: filterModel.updateFilter()
            }
            
            ComboBox {
                id: platformFilter
                model: [qsTr("全部平台 / All"), "微信", "抖音", "小红书", "B 站", "微博"]
                currentIndex: 0
                
                onCurrentIndexChanged: filterModel.updateFilter()
            }
            
            ComboBox {
                id: typeFilter
                model: [qsTr("全部类型 / All"), qsTr("文章 / Article"), qsTr("视频 / Video"), qsTr("图文 / Image")]
                currentIndex: 0
                
                onCurrentIndexChanged: filterModel.updateFilter()
            }
            
            Button {
                text: qsTr("🔍 搜索 / Search")
                
                onClicked: filterModel.updateFilter()
            }
        }
        
        // Content Table
        TableView {
            id: contentTable
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            
            model: ListModel {
                id: contentModel
                
                ListElement { 
                    title: "如何做出爆款内容？这 5 个技巧你必须知道"
                    platform: "微信"; views: 125000; likes: 8500; date: "2024-01-15"
                }
                ListElement { 
                    title: "抖音热门短视频分析：为什么这条视频火了？"
                    platform: "抖音"; views: 2500000; likes: 180000; date: "2024-01-14"
                }
                ListElement { 
                    title: "小红书笔记爆火的秘密：封面设计攻略"
                    platform: "小红书"; views: 89000; likes: 12000; date: "2024-01-13"
                }
                ListElement { 
                    title: "B 站 UP 主必看：提升播放量的 10 个方法"
                    platform: "B 站"; views: 450000; likes: 35000; date: "2024-01-12"
                }
                ListElement { 
                    title: "微博热搜背后的运营逻辑深度解析"
                    platform: "微博"; views: 780000; likes: 25000; date: "2024-01-11"
                }
            }
            
            delegate: Rectangle {
                width: implicitWidth
                height: 50
                color: index % 2 === 0 ? "white" : Material.color(Material.Gray, Material.Shade50)
                
                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 12
                    
                    Label {
                        Layout.preferredWidth: 400
                        text: model.title
                        elide: Text.ElideRight
                    }
                    
                    Label {
                        Layout.preferredWidth: 80
                        text: model.platform
                    }
                    
                    Label {
                        Layout.preferredWidth: 100
                        text: model.views >= 1000000 ? (model.views / 1000000).toFixed(1) + "M" : (model.views / 1000).toFixed(0) + "K"
                    }
                    
                    Label {
                        Layout.preferredWidth: 80
                        text: model.likes >= 10000 ? (model.likes / 1000).toFixed(1) + "K" : model.likes
                        color: "red"
                    }
                    
                    Label {
                        Layout.preferredWidth: 100
                        text: model.date
                        color: "gray"
                    }
                    
                    Button {
                        text: qsTr("分析 / Analyze")
                        
                        onClicked: {
                            // Trigger analysis
                        }
                    }
                }
            }
        }
        
        // Pagination
        RowLayout {
            Layout.fillWidth: true
            
            Label {
                text: qsTr("共 1234 条记录 / Total 1234 items")
                color: "gray"
            }
            
            Item { Layout.fillWidth: true }
            
            Button { text: "◀"; enabled: false }
            Button { text: "1"; flat: false }
            Button { text: "2" }
            Button { text: "3" }
            Label { text: "..."; anchors.verticalCenter: parent.verticalCenter }
            Button { text: "62" }
            Button { text: "▶" }
            
            Label {
                text: qsTr("每页显示 / Per page:")
                anchors.verticalCenter: parent.verticalCenter
            }
            
            SpinBox {
                value: 20
                from: 10
                to: 100
                stepSize: 10
            }
        }
    }
}
