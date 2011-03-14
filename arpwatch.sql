DROP TABLE IF EXISTS arpwatch;
CREATE TABLE arpwatch (
	timestamp	INTEGER(10),
	mac		CHAR(12) NOT NULL default '',
	ip		CHAR(39) NOT NULL default '',
	INDEX (mac),
	INDEX (ip),
	PRIMARY KEY (mac, ip)
) ENGINE=InnoDB;
