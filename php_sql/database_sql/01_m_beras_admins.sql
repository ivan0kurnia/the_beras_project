DROP TABLE IF EXISTS `u216319883_beras_m_db`.`beras_admins`;
CREATE TABLE `u216319883_beras_m_db`.`beras_admins`
(
    `admin_id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `admin_username` VARCHAR(30) NOT NULL,
    `admin_password` VARCHAR(30) NOT NULL,
    `admin_firstname` VARCHAR(35) NOT NULL,
    `admin_lastname` VARCHAR(35) NULL,
    `admin_email` VARCHAR(255) NULL,
    `admin_phone_number` VARCHAR(20) NULL,
    PRIMARY KEY (`admin_id`),
    UNIQUE `UNIQUE_USERNAME` (`admin_username`)
) ENGINE = InnoDB;

INSERT INTO `u216319883_beras_m_db`.`beras_admins`
(
    `admin_username`,
    `admin_password`,
    `admin_firstname`,
    `admin_lastname`,
    `admin_email`,
    `admin_phone_number`
)
VALUES
( -- 1
    'reynaldijavica',
    'godlike',
    'Reynaldi',
    'Javica',
    'reynaldijavica@gmail.com',
    '081284000465'
),
( -- 2
    'ed_ch98',
    'windows10',
    'Edward',
    'Chandra',
    'edwardchandra773@gmail.com',
    '08999937202'
);