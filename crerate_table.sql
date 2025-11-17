CREATE TABLE tb_user_info (
    id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY COMMENT '用户ID',
    user_name VARCHAR(50) UNIQUE NOT NULL COMMENT '用户名',
    email VARCHAR(100) UNIQUE NOT NULL COMMENT '邮箱',
    phone VARCHAR(20) UNIQUE NOT NULL COMMENT '手机号',
    password_hash VARCHAR(255) NOT NULL COMMENT '密码哈希，使用bcrypt算法',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间',
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间',
    
    INDEX (user_name),
    INDEX (email), 
    INDEX (phone)
) COMMENT='用户信息数据表';

CREATE TABLE tb_conversation_info (
    id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY COMMENT '会话ID',
    user_name VARCHAR(50) NOT NULL COMMENT '用户名',
    conversation_name VARCHAR(200) NOT NULL COMMENT '会话名称',
    last_message_time TIMESTAMP NULL COMMENT '最后消息时间',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间',
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间',

    INDEX (user_name, created_at),
    INDEX (user_name, last_message_time),
    
    FOREIGN KEY (user_name) REFERENCES tb_user_info(user_name) ON DELETE RESTRICT
) COMMENT='聊天会话信息数据表\n外键约束：要求先清空会话记录才能删除对应用户';

CREATE TABLE tb_chat_message_info (
    id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY COMMENT '消息ID',
    conversation_id BIGINT UNSIGNED NOT NULL COMMENT '会话ID',
    user_name VARCHAR(50) NOT NULL COMMENT '用户名',
    message_type VARCHAR(20) NOT NULL COMMENT '消息类型：user/assistant/system',
    message_content TEXT NOT NULL COMMENT '消息内容',
    token_count INT UNSIGNED DEFAULT 0 COMMENT '消息的token数量',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间',
    
    INDEX (conversation_id, created_at),
    INDEX (user_name, created_at),
    
    FOREIGN KEY (conversation_id) REFERENCES tb_conversation_info(id) ON DELETE RESTRICT,
    FOREIGN KEY (user_name) REFERENCES tb_user_info(user_name) ON DELETE RESTRICT
) COMMENT='聊天消息信息表\n外键约束：要求先清空所有消息记录才能删除对应的会话和用户';