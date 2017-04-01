#define EPSILON_TEMP 0.2
#define EQUILIBRIO_TERM 1

typedef struct temp {
  double valor;
  double factor;
} TEMPERATURA;

TEMPERATURA* init_temperatura(TEMPERATURA *temp, double t, double p);
TEMPERATURA* modificar_temperatura(TEMPERATURA *t);
