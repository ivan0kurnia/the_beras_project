DROP TABLE IF EXISTS `u216319883_beras_m_db`.`beras_users`;
CREATE TABLE `u216319883_beras_m_db`.`beras_users`
(
    `user_id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `card_uid` CHAR(14) NOT NULL,
    `user_ktp` CHAR(16) NOT NULL,
    `user_firstname` VARCHAR(35) NOT NULL,
    `user_lastname` VARCHAR(35) NULL,
    `user_gender` ENUM('L', 'P') NOT NULL,
    `user_address_current` VARCHAR(255) NOT NULL,
    `user_landline_number` VARCHAR(20) NULL,
    `user_cellphone_number` VARCHAR(20) NULL,
    `user_liability` TINYINT UNSIGNED NOT NULL,
    `user_staple_quota` SMALLINT UNSIGNED NOT NULL,
    `user_creation_date` DATETIME NOT NULL DEFAULT NOW(),
    PRIMARY KEY (`user_id`),
    UNIQUE `UNIQUE_USER_ID` (`user_id`),
    UNIQUE `UNIQUE_UID` (`card_uid`),
    UNIQUE `UNIQUE_KTP` (`user_ktp`)
) ENGINE = InnoDB;