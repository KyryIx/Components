CREATE TABLE components (
    id                 INTEGER       PRIMARY KEY AUTOINCREMENT
                                     NOT NULL,
    name               VARCHAR (255) NOT NULL,
    description        VARCHAR (255) NOT NULL,
    types              VARCHAR (15)  DEFAULT ('Outro'),
    quantity           INTEGER       NOT NULL,
    locationImage      VARCHAR (255),
    locationBox        VARCHAR (255),
    locationDatasheet  VARCHAR (255),
    locationAttachment VARCHAR (255),
    status             CHAR (1)      DEFAULT ('y') 
);

