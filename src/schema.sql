CREATE TABLE channel (
    server_name VARCHAR(50) NOT NULL,
    channel VARCHAR(50) NOT NULL,
    active BIT NOT NULL DEFAULT 1
);
CREATE TABLE config (
    key VARCHAR(100),
    value VARCHAR(100)
);
CREATE TABLE plugin (
    path VARCHAR(1000) NOT NULL,
    active BIT NOT NULL DEFAULT 1
);
CREATE TABLE role (
    name VARCHAR(50) NOT NULL,
    description VARCHAR(200)
);
CREATE TABLE server (
    name VARCHAR(50) NOT NULL,
    host VARCHAR(64) NOT NULL,
    port VARCHAR(5) NOT NULL,
    nick VARCHAR(50) NOT NULL,
    active BIT NOT NULL DEFAULT 1
);
CREATE TABLE user (
    nick VARCHAR(50) NOT NULL,
    password VARCHAR(50) NOT NULL
);
CREATE TABLE user_role_mapping (
    role VARCHAR(50),
    nick VARCHAR(50)
);

INSERT INTO role (name, description) VALUES ('guest', 'Guest privileges');
INSERT INTO role (name, description) VALUES ('admin', 'Administrative privileges');
