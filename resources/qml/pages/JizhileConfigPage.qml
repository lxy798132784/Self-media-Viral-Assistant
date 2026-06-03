import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

/**
 * @brief 极致了数据源配置页面
 *        Jizhile Data Source Configuration Page
 *
 * @details 提供完整的可视化界面用于配置极致了平台的所有参数。
 *          包括鉴权信息、平台选择、设备指纹、地域定向、指标筛选等。
 *          Provides a complete visual interface for configuring all Jizhile platform parameters.
 *          Includes authentication, platform selection, device fingerprints, 
 *          geo-targeting, metrics filtering, etc.
 *
 * @note 所有修改实时生效，无需重启应用
 *       All changes take effect in real-time without restarting the application
 */
ScrollView {
    id: jizhileConfigPage
    
    /**
     * @brief 配置数据模型
     *        Configuration data model
     */
    property var configData: ({
        // 基础鉴权
        bearer_token: "",
        timeout_seconds: 30,
        max_retries: 3,
        
        // 平台选择
        enabled_platforms: ["wechat", "douyin", "xiaohongshu"],
        
        // 设备配置
        preferred_device: "ios",
        os_version_filter: "",
        device_brand_filter: "",
        
        // 地域定向
        target_provinces: [],
        target_cities: [],
        exclude_abnormal_traffic: true,
        
        // 时间粒度
        time_granularity: "daily",
        history_days: 30,
        
        // 指标筛选
        metrics_filter: {
            min_read_count: 0,
            min_like_count: 0,
            min_comment_count: 0,
            min_share_count: 0,
            min_collect_count: 0,
            min_engagement_rate: 0.0
        },
        
        // 高级选项
        enable_real_time_monitoring: false,
        real_time_interval_seconds: 300,
        custom_user_agent: "",
        enable_cache: true,
        cache_ttl_hours: 24
    })
    
    /**
     * @brief 保存配置信号
     *        Save configuration signal
     *
     * @param config 配置对象 / Configuration object
     */
    signal configSaved(var config)
    
    /**
     * @brief 测试连接信号
     *        Test connection signal
     */
    signal testConnectionRequested()
    
    ColumnLayout {
        width: parent.width
        spacing: 20
        
        // === 基础鉴权设置 / Basic Authentication ===
        GroupBox {
            title: qsTr("基础鉴权 / Basic Authentication")
            Layout.fillWidth: true
            
            ColumnLayout {
                anchors.fill: parent
                spacing: 15
                
                // Bearer Token
                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("Bearer Token:")
                        Layout.preferredWidth: 150
                        font.bold: true
                    }
                    TextField {
                        id: bearerTokenField
                        Layout.fillWidth: true
                        placeholderText: qsTr("请输入 Bearer Token / Enter Bearer Token")
                        echoMode: TextInput.Password
                        text: configData.bearer_token
                        
                        onTextChanged: configData.bearer_token = text
                    }
                    ToolButton {
                        icon.name: "view-conceal"
                        onClicked: {
                            bearerTokenField.echoMode = 
                                bearerTokenField.echoMode === TextInput.Password 
                                ? TextInput.Normal 
                                : TextInput.Password
                        }
                        ToolTip.text: qsTr("显示/隐藏密码 / Show/Hide Password")
                    }
                }
                
                // 超时时间
                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("请求超时 (秒) / Request Timeout (s):")
                        Layout.preferredWidth: 150
                    }
                    SpinBox {
                        id: timeoutSpinBox
                        Layout.fillWidth: true
                        from: 5
                        to: 300
                        value: configData.timeout_seconds
                        onValueChanged: configData.timeout_seconds = value
                    }
                }
                
                // 最大重试次数
                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("最大重试次数 / Max Retries:")
                        Layout.preferredWidth: 150
                    }
                    SpinBox {
                        id: retriesSpinBox
                        Layout.fillWidth: true
                        from: 0
                        to: 10
                        value: configData.max_retries
                        onValueChanged: configData.max_retries = value
                    }
                }
            }
        }
        
        // === 平台选择 / Platform Selection ===
        GroupBox {
            title: qsTr("启用平台 / Enabled Platforms")
            Layout.fillWidth: true
            
            ColumnLayout {
                anchors.fill: parent
                spacing: 10
                
                Flow {
                    Layout.fillWidth: true
                    spacing: 15
                    
                    Repeater {
                        model: [
                            {id: "wechat", name: qsTr("微信公众号 / WeChat")},
                            {id: "douyin", name: qsTr("抖音 / Douyin")},
                            {id: "xiaohongshu", name: qsTr("小红书 / RED")},
                            {id: "weibo", name: qsTr("微博 / Weibo")},
                            {id: "kuaishou", name: qsTr("快手 / Kuaishou")},
                            {id: "bilibili", name: qsTr("B 站 / Bilibili")},
                            {id: "toutiao", name: qsTr("今日头条 / Toutiao")},
                            {id: "zhihu", name: qsTr("知乎 / Zhihu")}
                        ]
                        
                        delegate: CheckBox {
                            text: modelData.name
                            checked: configData.enabled_platforms.indexOf(modelData.id) !== -1
                            onCheckedChanged: {
                                if (checked) {
                                    if (configData.enabled_platforms.indexOf(modelData.id) === -1) {
                                        configData.enabled_platforms.push(modelData.id)
                                    }
                                } else {
                                    var idx = configData.enabled_platforms.indexOf(modelData.id)
                                    if (idx !== -1) {
                                        configData.enabled_platforms.splice(idx, 1)
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        // === 设备集群配置 / Device Cluster Configuration ===
        GroupBox {
            title: qsTr("设备配置 / Device Configuration")
            Layout.fillWidth: true
            
            ColumnLayout {
                anchors.fill: parent
                spacing: 15
                
                // 首选设备类型
                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("首选设备 / Preferred Device:")
                        Layout.preferredWidth: 150
                        font.bold: true
                    }
                    ComboBox {
                        id: deviceTypeCombo
                        Layout.fillWidth: true
                        model: [
                            qsTr("iOS 设备 / iOS Device"),
                            qsTr("Android 设备 / Android Device"),
                            qsTr("PC 网页 / PC Web"),
                            qsTr("移动网页 / Mobile Web")
                        ]
                        currentIndex: {
                            switch(configData.preferred_device) {
                                case "ios": return 0
                                case "android": return 1
                                case "pc_web": return 2
                                case "mobile_web": return 3
                                default: return 0
                            }
                        }
                        onCurrentIndexChanged: {
                            switch(currentIndex) {
                                case 0: configData.preferred_device = "ios"; break
                                case 1: configData.preferred_device = "android"; break
                                case 2: configData.preferred_device = "pc_web"; break
                                case 3: configData.preferred_device = "mobile_web"; break
                            }
                        }
                    }
                }
                
                // OS 版本过滤
                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("OS 版本过滤 / OS Version Filter:")
                        Layout.preferredWidth: 150
                    }
                    TextField {
                        Layout.fillWidth: true
                        placeholderText: qsTr("例如：15.0-17.0 / e.g., 15.0-17.0")
                        text: configData.os_version_filter
                        onTextChanged: configData.os_version_filter = text
                    }
                }
                
                // 设备品牌过滤
                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("设备品牌 / Device Brand:")
                        Layout.preferredWidth: 150
                    }
                    TextField {
                        Layout.fillWidth: true
                        placeholderText: qsTr("例如：Apple,Huawei,Xiaomi / e.g., Apple,Huawei,Xiaomi")
                        text: configData.device_brand_filter
                        onTextChanged: configData.device_brand_filter = text
                    }
                }
            }
        }
        
        // === 地域定向 / Geo-Targeting ===
        GroupBox {
            title: qsTr("地域定向 / Geo-Targeting")
            Layout.fillWidth: true
            
            ColumnLayout {
                anchors.fill: parent
                spacing: 15
                
                // 目标省份
                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("目标省份 / Target Provinces:")
                        Layout.preferredWidth: 150
                        font.bold: true
                    }
                    TextField {
                        Layout.fillWidth: true
                        placeholderText: qsTr("多个省份用逗号分隔 / Comma-separated")
                        text: configData.target_provinces.join(", ")
                        onTextChanged: {
                            configData.target_provinces = text.split(",").map(s => s.trim()).filter(s => s.length > 0)
                        }
                    }
                }
                
                // 目标城市
                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("目标城市 / Target Cities:")
                        Layout.preferredWidth: 150
                    }
                    TextField {
                        Layout.fillWidth: true
                        placeholderText: qsTr("多个城市用逗号分隔 / Comma-separated")
                        text: configData.target_cities.join(", ")
                        onTextChanged: {
                            configData.target_cities = text.split(",").map(s => s.trim()).filter(s => s.length > 0)
                        }
                    }
                }
                
                // 排除异常流量
                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("排除异常流量 / Exclude Abnormal Traffic:")
                        Layout.preferredWidth: 150
                    }
                    Switch {
                        checked: configData.exclude_abnormal_traffic
                        onCheckedChanged: configData.exclude_abnormal_traffic = checked
                    }
                }
            }
        }
        
        // === 数据采集粒度 / Data Collection Granularity ===
        GroupBox {
            title: qsTr("采集设置 / Collection Settings")
            Layout.fillWidth: true
            
            ColumnLayout {
                anchors.fill: parent
                spacing: 15
                
                // 时间粒度
                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("时间粒度 / Time Granularity:")
                        Layout.preferredWidth: 150
                        font.bold: true
                    }
                    ComboBox {
                        id: granularityCombo
                        Layout.fillWidth: true
                        model: [
                            qsTr("小时级 / Hourly"),
                            qsTr("天级 / Daily"),
                            qsTr("周级 / Weekly"),
                            qsTr("月级 / Monthly")
                        ]
                        currentIndex: {
                            switch(configData.time_granularity) {
                                case "hourly": return 0
                                case "daily": return 1
                                case "weekly": return 2
                                case "monthly": return 3
                                default: return 1
                            }
                        }
                        onCurrentIndexChanged: {
                            switch(currentIndex) {
                                case 0: configData.time_granularity = "hourly"; break
                                case 1: configData.time_granularity = "daily"; break
                                case 2: configData.time_granularity = "weekly"; break
                                case 3: configData.time_granularity = "monthly"; break
                            }
                        }
                    }
                }
                
                // 历史天数
                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("历史数据天数 / History Days:")
                        Layout.preferredWidth: 150
                    }
                    SpinBox {
                        id: historyDaysSpinBox
                        Layout.fillWidth: true
                        from: 1
                        to: 365
                        value: configData.history_days
                        onValueChanged: configData.history_days = value
                    }
                }
            }
        }
        
        // === 指标筛选 / Metrics Filtering ===
        GroupBox {
            title: qsTr("指标筛选 / Metrics Filtering")
            Layout.fillWidth: true
            
            ColumnLayout {
                anchors.fill: parent
                spacing: 15
                
                // 最小阅读量
                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("最小阅读量 / Min Read Count:")
                        Layout.preferredWidth: 150
                    }
                    SpinBox {
                        Layout.fillWidth: true
                        from: 0
                        to: 10000000
                        value: configData.metrics_filter.min_read_count
                        onValueChanged: configData.metrics_filter.min_read_count = value
                    }
                }
                
                // 最小点赞数
                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("最小点赞数 / Min Like Count:")
                        Layout.preferredWidth: 150
                    }
                    SpinBox {
                        Layout.fillWidth: true
                        from: 0
                        to: 1000000
                        value: configData.metrics_filter.min_like_count
                        onValueChanged: configData.metrics_filter.min_like_count = value
                    }
                }
                
                // 最小评论数
                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("最小评论数 / Min Comment Count:")
                        Layout.preferredWidth: 150
                    }
                    SpinBox {
                        Layout.fillWidth: true
                        from: 0
                        to: 100000
                        value: configData.metrics_filter.min_comment_count
                        onValueChanged: configData.metrics_filter.min_comment_count = value
                    }
                }
                
                // 最小互动率
                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("最小互动率 (%) / Min Engagement Rate (%):")
                        Layout.preferredWidth: 150
                    }
                    DoubleSpinBox {
                        Layout.fillWidth: true
                        from: 0.0
                        to: 100.0
                        decimals: 2
                        value: configData.metrics_filter.min_engagement_rate
                        onValueChanged: configData.metrics_filter.min_engagement_rate = value
                    }
                }
            }
        }
        
        // === 高级选项 / Advanced Options ===
        GroupBox {
            title: qsTr("高级选项 / Advanced Options")
            Layout.fillWidth: true
            
            ColumnLayout {
                anchors.fill: parent
                spacing: 15
                
                // 实时监控
                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("启用实时监控 / Enable Real-time Monitoring:")
                        Layout.preferredWidth: 150
                        font.bold: true
                    }
                    Switch {
                        checked: configData.enable_real_time_monitoring
                        onCheckedChanged: configData.enable_real_time_monitoring = checked
                    }
                }
                
                // 监控间隔
                RowLayout {
                    Layout.fillWidth: true
                    visible: configData.enable_real_time_monitoring
                    Label {
                        text: qsTr("监控间隔 (秒) / Monitoring Interval (s):")
                        Layout.preferredWidth: 150
                    }
                    SpinBox {
                        Layout.fillWidth: true
                        from: 60
                        to: 3600
                        value: configData.real_time_interval_seconds
                        onValueChanged: configData.real_time_interval_seconds = value
                    }
                }
                
                // 自定义 User-Agent
                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("自定义 User-Agent:")
                        Layout.preferredWidth: 150
                    }
                    TextField {
                        Layout.fillWidth: true
                        placeholderText: qsTr("留空使用默认值 / Leave empty for default")
                        text: configData.custom_user_agent
                        onTextChanged: configData.custom_user_agent = text
                    }
                }
                
                // 启用缓存
                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: qsTr("启用本地缓存 / Enable Local Cache:")
                        Layout.preferredWidth: 150
                    }
                    Switch {
                        checked: configData.enable_cache
                        onCheckedChanged: configData.enable_cache = checked
                    }
                }
                
                // 缓存有效期
                RowLayout {
                    Layout.fillWidth: true
                    visible: configData.enable_cache
                    Label {
                        text: qsTr("缓存有效期 (小时) / Cache TTL (hours):")
                        Layout.preferredWidth: 150
                    }
                    SpinBox {
                        Layout.fillWidth: true
                        from: 1
                        to: 168
                        value: configData.cache_ttl_hours
                        onValueChanged: configData.cache_ttl_hours = value
                    }
                }
            }
        }
        
        // === 操作按钮 / Action Buttons ===
        RowLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignRight
            
            Button {
                text: qsTr("测试连接 / Test Connection")
                icon.name: "network-wired"
                onClicked: testConnectionRequested()
            }
            
            Item { Layout.fillWidth: true }
            
            Button {
                text: qsTr("重置 / Reset")
                icon.name: "edit-undo"
                onClicked: {
                    // 重置为默认值
                    configData.bearer_token = ""
                    configData.timeout_seconds = 30
                    configData.max_retries = 3
                    configData.enabled_platforms = ["wechat", "douyin", "xiaohongshu"]
                    configData.preferred_device = "ios"
                    configData.os_version_filter = ""
                    configData.device_brand_filter = ""
                    configData.target_provinces = []
                    configData.target_cities = []
                    configData.exclude_abnormal_traffic = true
                    configData.time_granularity = "daily"
                    configData.history_days = 30
                    configData.metrics_filter = {
                        min_read_count: 0,
                        min_like_count: 0,
                        min_comment_count: 0,
                        min_share_count: 0,
                        min_collect_count: 0,
                        min_engagement_rate: 0.0
                    }
                    configData.enable_real_time_monitoring = false
                    configData.real_time_interval_seconds = 300
                    configData.custom_user_agent = ""
                    configData.enable_cache = true
                    configData.cache_ttl_hours = 24
                }
            }
            
            Button {
                text: qsTr("保存配置 / Save Config")
                icon.name: "document-save"
                highlighted: true
                onClicked: {
                    configSaved(configData)
                }
            }
        }
    }
}
