# metnum-tp3

### Formato de csv
csv_converter agarra una imagen y la modifica a imagen cuadrada de tamaño min(alto, ancho)
el archivo que genera tiene el formato:
```
size
p(0,0), p(0,1), ..., p(0,size-1) <-- sin coma al final
p(1,0), ...
.
.
.
p(n,0), p(n,1), ..., p(size-1,size-1)
EOF
```
### Generación de rayos

Código sacado de [acá](http://eugen.dedu.free.fr/projects/bresenham/)
