/****************************************************************************
**
** Core Types Definition / 核心类型定义
**
** Copyright (C) 2024 Self-media-Viral-Assistant Team.
** Contact: https://github.com/lxy798132784/Self-media-Viral-Assistant
**
** This file is part of the Core Layer.
**
** Commercial License Usage
** Licensees holding valid commercial licenses may use this file in
** accordance with the commercial license agreement provided.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation.
**
****************************************************************************/

#pragma once

#include <QString>
#include <QDateTime>
#include <QVariantMap>
#include <QMetaType>
#include <memory>
#include <vector>
#include <optional>

/**
 * @brief Error information structure / 错误信息结构体
 * 
 * Contains error code, message, and optional details for debugging.
 * 包含错误代码、消息和可选的调试详细信息。
 */
struct ErrorInfo {
    int code = 0;                 ///< Error code / 错误代码
    QString message;              ///< Human-readable message / 人类可读消息
    QString details;              ///< Detailed debug info / 详细调试信息
    QString source;               ///< Source module name / 来源模块名

    bool isValid() const { return code != 0; }
    
    static ErrorInfo success() { return {}; }
    static ErrorInfo failure(int c, const QString& msg, const QString& src = {}) {
        return {c, msg, {}, src};
    }
};

/**
 * @brief Content metrics data / 内容指标数据
 * 
 * Unified structure for likes, views, comments, shares, etc.
 * 统一结构体，包含点赞、浏览、评论、分享等数据。
 */
struct MetricsResult {
    qint64 views = 0;             ///< View count / 浏览量
    qint64 likes = 0;             ///< Like count / 点赞数
    qint64 comments = 0;          ///< Comment count / 评论数
    qint64 shares = 0;            ///< Share count / 分享数
    qint64 favorites = 0;         ///< Favorite count / 收藏数
    double engagementRate = 0.0;  ///< Engagement rate (%) / 互动率

    QVariantMap raw_data;         ///< Raw JSON data / 原始JSON数据
};

/**
 * @brief Platform enumeration / 平台枚举
 */
enum class Platform {
    Unknown = 0,
    WeChatChannels,   ///< 视频号
    WeChatOfficial,   ///< 公众号
    Douyin,           ///< 抖音
    Xiaohongshu,      ///< 小红书
    Bilibili,         ///< B站
    Kuaishou,         ///< 快手
    Weibo,            ///< 微博
    Toutiao,          ///< 头条
    Zhihu             ///< 知乎
};

/**
 * @brief Content item structure / 内容项结构体
 * 
 * Represents a single piece of content (video/article) from any platform.
 * 代表来自任何平台的单条内容（视频/文章）。
 */
struct ContentItem {
    QString id;                   ///< Unique ID / 唯一ID
    QString title;                ///< Title / 标题
    QString description;          ///< Description / 描述
    QString author;               ///< Author name / 作者名
    QString authorId;             ///< Author ID / 作者ID
    Platform platform = Platform::Unknown; ///< Platform / 平台
    QString url;                  ///< Content URL / 内容链接
    QString coverUrl;             ///< Cover image URL / 封面图链接
    QDateTime publishTime;        ///< Publish time / 发布时间
    QString duration;             ///< Duration (video) / 时长 (视频)
    
    MetricsResult metrics;        ///< Metrics / 指标数据
    QVariantMap extraData;        ///< Platform-specific data / 平台特有数据
    
    bool isValid() const { return !id.isEmpty(); }
};

/**
 * @brief Search parameters / 搜索参数
 */
struct SearchParams {
    QString keyword;              ///< Search keyword / 搜索关键词
    Platform platform = Platform::Unknown; ///< Target platform / 目标平台
    int page = 1;                 ///< Page number / 页码
    int pageSize = 20;            ///< Items per page / 每页数量
    QString sortBy = "relevance"; ///< Sort by: relevance, latest, hottest / 排序方式
    QString timeRange = "all";    ///< Time range: all, day, week, month / 时间范围
    QString regionCode;           ///< Region code (e.g., CN-GD) / 地区代码
    QVariantMap extraParams;      ///< Extra params (device_id, etc.) / 额外参数
};

/**
 * @brief Result wrapper for async operations / 异步操作结果包装器
 * 
 * Uses std::optional to simulate std::expected (C++23) in C++20.
 * 使用 std::optional 模拟 C++23 的 std::expected，兼容 C++20。
 */
template<typename T>
using Result = std::pair<std::optional<T>, ErrorInfo>;

template<typename T>
Result<T> make_success(const T& value) {
    return {value, ErrorInfo::success()};
}

template<typename T>
Result<T> make_failure(const ErrorInfo& err) {
    return {std::nullopt, err};
}

// Register types for Qt Meta-Object System / 注册 Qt 元对象系统类型
void registerCoreTypes();
