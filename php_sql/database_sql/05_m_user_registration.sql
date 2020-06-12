DROP TABLE IF EXISTS `u216319883_beras_m_db`.`user_registration`;
CREATE TABLE `u216319883_beras_m_db`.`user_registration`
(
    `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `name` VARCHAR(255) NOT NULL,
    `email` VARCHAR(255) NOT NULL,
    `password` VARCHAR(255) NOT NULL,
    PRIMARY KEY (`id`)
) ENGINE = InnoDB;