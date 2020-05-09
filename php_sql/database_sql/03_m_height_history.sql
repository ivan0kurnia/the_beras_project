DROP TABLE IF EXISTS `u216319883_beras_m_db`.`height_history`;
CREATE TABLE `u216319883_beras_m_db`.`height_history`
(
    `height_id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `height_percentage` ENUM('15', '20', '40', '60', '80') NOT NULL,
    `height_datetime` DATETIME NOT NULL DEFAULT NOW(),
    PRIMARY KEY (`height_id`)
) ENGINE = InnoDB;