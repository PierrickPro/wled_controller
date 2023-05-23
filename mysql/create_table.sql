CREATE TABLE WLED_STATE (
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    ip VARCHAR(15),
    json_state TEXT,
    click_type INT,
    controller_id CHAR(20),
    PRIMARY KEY (controller_id, click_type)
);