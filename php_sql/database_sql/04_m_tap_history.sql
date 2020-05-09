DROP TABLE IF EXISTS `u216319883_beras_m_db`.`tap_history`;
CREATE TABLE `u216319883_beras_m_db`.`tap_history`
(
    `tap_id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `card_uid` CHAR(14) NOT NULL,
    `tap_datetime` DATETIME NOT NULL DEFAULT NOW(),
    PRIMARY KEY (`tap_id`),
    FOREIGN KEY (`card_uid`) REFERENCES `u216319883_beras_m_db`.`beras_users` (`card_uid`)
) ENGINE = InnoDB;