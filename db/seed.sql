-- Datos iniciales del sistema
INSERT OR IGNORE INTO categorias (nombre, descripcion) VALUES
  ('Motor', 'Pistones, cilindros, válvulas, bielas'),
  ('Frenos', 'Pastillas, discos, líquido de frenos, manetas'),
  ('Lubricantes', 'Aceites de motor, transmisión, filtros'),
  ('Llantas', 'Llantas y cámaras'),
  ('Eléctrico', 'Baterías, bujías, faros, reguladores'),
  ('Transmisión', 'Cadenas, piñones, coronas, kit de arrastre'),
  ('Accesorios', 'Maleteros, espejos, alforjas, tapicería'),
  ('Carenaje', 'Guardafangos, tanques, laterales');

INSERT OR IGNORE INTO marcas (nombre) VALUES
  ('Yamaha'), ('Honda'), ('Suzuki'), ('Bajaj'),
  ('AKT'), ('KTM'), ('Kawasaki'), ('TVS');
