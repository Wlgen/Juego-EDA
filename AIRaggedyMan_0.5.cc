#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME RaggedyMan


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }
  /**
   * Types and attributes for your player can be defined here.
   */
  typedef vector< vector<int> >  mat_dist;
  typedef vector< vector<Pos> >  mat_pos;

  //Busca la celda de agua y de ciudad mas cercana a la posicion guerrero y devuelve dos matrices,
  //Una con las distancias que hay hasta cada celda, otra con la celda siguiente para recorrer el
  //camino mas corto hasta la posicion guerrero, y la posicion de la celda de agua y ciudad mas cercana;
  void busca_guerrero(mat_dist& distan, mat_pos& prev, Pos& ag, Pos& ciu, Pos guerrero){
    int n = rows()*cols();
    bool agua = false, ciudad = false;
    queue<Pos> q;
    distan = mat_dist(rows(),vector<int> (cols(),n)); //matriz para guardar las distancias a la posicion inicial
    prev = mat_pos(rows(), vector<Pos> (cols(),Pos(-1,-1)));//matriz para guardar la posicion anterior del camino mas corto a la posicion inicial
    vector< vector<bool> > visto(rows(), vector<bool>(cols(),false));
    q.push(guerrero);
    distan[guerrero.i][guerrero.j] = 0;
    while(not q.empty() and (not agua or not ciudad)){ //bfs para encontrar las cosas
      Pos posact;
      posact = q.front(); q.pop();
      visto[posact.i][posact.j] = true;
      for(int i = 0; i < 8; ++i){
        Pos next;
        switch(i){//una accion para cada posicion a la que se pueda mover desde posact
          case 0:
            next = posact + Top;
            break;

          case 1:
            next = posact + RT;
            break;

          case 2:
            next = posact + Right;
            break;

          case 3:
            next = posact + BR;
            break;

          case 4:
            next = posact + Bottom;
            break;

          case 5:
            next = posact + LB;
            break;

          case 6:
            next = posact + Left;
            break;

          case 7:
            next = posact + TL;
            break;
          }
          if(pos_ok(next)){
            if(not visto[next.i][next.j]){//si el nodo no se ha visitado
              Cell cellu = cell(next);
              visto[next.i][next.j] = true;
              if(cellu.type != Wall and cellu.id == -1 and cellu.type != Water){//si no es un muro ni hay un guerrero ni es agua
                q.push(next);
                distan[next.i][next.j] = distan[posact.i][posact.j]+1;
                prev[next.i][next.j] = posact;
                if(cellu.type == City){
                  ciudad = true;
                  ciu = next;
                }
              }
              else{
                if(cellu.type == Water and not agua){//si el siguente es agua guarda la posicion actual(celda adyacente mas cercana)
                  agua = true;
                  ag = posact;
                }
              }
            }
          }
        }
      }
    }


  //decide solo para la mitad de los guerreros
  void guerreros_mitad(vector<int>& num_guerr, int a){
    for(int i = a; i < int(num_guerr.size()); i += 2){ //Bucle para comprobar por cada guerrero en mi posesión
      int guerr_id = num_guerr[i];
      Pos posid = unit(guerr_id).pos;
      Pos agua,ciudad;//posicion de la casilla de agua y de la casilla de ciudad mas cercana
      Cell poscell = cell(posid);
      mat_dist distancia;
      mat_pos pos_anterior;
      busca_guerrero(distancia,pos_anterior,agua,ciudad,posid);//llama a busca_guerrero para saber la posicion y la distancia a la casilla de agua y de ciudad mas cercanas
      Pos next(-1,-1);
      if(poscell.type == City){
        if(unit(guerr_id).water < 30){
          next = agua;
          while(pos_anterior[next.i][next.j] != posid){
            next = pos_anterior[next.i][next.j];
          }
        }
      }
      else{
        if(unit(guerr_id).water < 30){
          next = agua;
          while(pos_anterior[next.i][next.j] != posid){
            next = pos_anterior[next.i][next.j];
          }
        }
        else{
          next = ciudad;
          while(pos_anterior[next.i][next.j] != posid){
            next = pos_anterior[next.i][next.j];
          }
        }
      }
      if(next == (posid + Top)) command(guerr_id,Top);
      else if(next == (posid + RT)) command(guerr_id,RT);
      else if(next == (posid + Right)) command(guerr_id,Right);
      else if(next == (posid + BR)) command(guerr_id,BR);
      else if(next == (posid + Bottom)) command(guerr_id,Bottom);
      else if(next == (posid + LB)) command(guerr_id,LB);
      else if(next == (posid + Left)) command(guerr_id,Left);
      else if(next == (posid + TL)) command(guerr_id,TL);
      else command(guerr_id,None);
      }
  }
  //Decide el movimiento que van a hacer cada guerrero
  void guerreros(){
    vector<int> num_guerr = warriors(me());
    for(int i = 0; i < int(num_guerr.size()); ++i){//comprueba si hay algun enemigo en una casilla contigua a cualquier guerrero, si lo hay que lo ataque;
      Unit act = unit(num_guerr[i]);
      int arriba = cell(act.pos + Top).id;
      int arr_der = cell(act.pos + RT).id;
      int derecha = cell(act.pos + Right).id;
      int ab_der = cell(act.pos + BR).id;
      int abajo = cell(act.pos + Bottom).id;
      int ab_izq = cell(act.pos + LB).id;
      int izquierda = cell(act.pos + Left).id;
      int arr_izq = cell(act.pos + TL).id;
      if(arriba != -1){
        if(unit(arriba).player != me()){
          command(num_guerr[i],Top);
        }
      }
      else if(arr_der != -1){
        if(unit(arr_der).player != me()){
          command(num_guerr[i],RT);
        }
      }
      else if(derecha != -1){
        if(unit(derecha).player != me()){
          command(num_guerr[i],Right);
        }
      }
      else if(ab_der != -1){
        if(unit(ab_der).player != me()){
          command(num_guerr[i],BR);
        }
      }
      else if(abajo != -1){
        if(unit(abajo).player != me()){
          command(num_guerr[i],Bottom);
        }
      }
      else if(ab_izq != -1){
        if(unit(ab_izq).player != me()){
          command(num_guerr[i],LB);
        }
      }
      else if(izquierda != -1){
        if(unit(izquierda).player != me()){
          command(num_guerr[i],Left);
        }
      }
      else if(arr_izq != -1){
        if(unit(arr_izq).player != me()){
          command(num_guerr[i],TL);
        }
      }
    }
    guerreros_mitad(num_guerr,0);
    guerreros_mitad(num_guerr,1);
  }

  void buscar_coche(mat_dist& distan, mat_pos& prev, Pos& fuel, Pos& enem, Pos coche){
    distan = mat_dist(rows(),vector<int>(cols(),rows()*cols()));
    prev = mat_pos(rows(),vector<Pos>(cols(),Pos(-1,-1)));
    vector< vector<bool> > visto(rows(),vector<bool>(cols(),false));
    queue<Pos> q;
    distan[coche.i][coche.j] = 0;
    q.push(coche);
    bool enemy = false, gas = false;
    while(not q.empty() and not enemy and not gas){
      Pos act = q.front(); q.pop();
      for(int i = 0; i < 8; ++i){
        Pos next;
        switch(i){
          case 0:
            next = act + Top;
            break;

          case 1:
            next = act + RT;
            break;

          case 2:
            next = act + Right;
            break;

          case 3:
            next = act + BR;
            break;

          case 4:
            next = act + Bottom;
            break;

          case 5:
            next = act + LB;
            break;

          case 6:
            next = act + Left;
            break;

          case 7:
            next = act + TL;
            break;
        }
        if(pos_ok(next)){//si es una posicion correcta
          if(not visto[next.i][next.j]){//si no se ha visitado ya la celda
            Cell cellu = cell(next);
            visto[next.i][next.j] = true;
            if(cellu.type != Wall and cellu.type != City and cellu.type != Station and cellu.type != Water){
              if(cellu.id != -1){
                q.push(next);
                distan[next.i][next.j] = distan[act.i][act.j]+1;
                prev[next.i][next.j] = act;
              }
              else{
                if(unit(cellu.id).player != me()){
                  q.push(next);
                  distan[next.i][next.j] = distan[act.i][act.j]+1;
                  prev[next.i][next.j] = act;
                  if(not enemy){
                    enemy = true;
                    enem = next;
                  }
                }
                if(cellu.type == Station and not gas){
                  gas = true;
                  fuel = act;
                }
              }
            }
          }
        }
      }
    }
  }

  void coches(){
    vector<int> num_coches = cars(me());
    for(int i = 0; i < int(num_coches.size()); ++i){

        int id_coche = num_coches[i];
        mat_dist distancia;
        mat_pos pos_anterior;
        Pos gasolina, enemigo, next;
      if(can_move(id_coche)){
        Pos pos_coche = unit(id_coche).pos;
        buscar_coche(distancia,pos_anterior,gasolina,enemigo,pos_coche);
        if(pos_ok(gasolina) and pos_ok(enemigo)){
          if(unit(id_coche).food + 1 < distancia[gasolina.i][gasolina.j]){
            next = gasolina;
            while(pos_anterior[next.i][next.j] != pos_coche){
              next = pos_anterior[next.i][next.j];
            }
          }
          else{
            next = enemigo;
            while(pos_anterior[next.i][next.j] != pos_coche){
              next = pos_anterior[next.i][next.j];
            }
          }
        }
        if(next == (pos_coche + Top)) command(id_coche,Top);
        else if(next == (pos_coche + RT)) command(id_coche,RT);
        else if(next == (pos_coche + Right)) command(id_coche,Right);
        else if(next == (pos_coche + BR)) command(id_coche,BR);
        else if(next == (pos_coche + Bottom)) command(id_coche,Bottom);
        else if(next == (pos_coche + LB)) command(id_coche,LB);
        else if(next == (pos_coche + Left)) command(id_coche,Left);
        else if(next == (pos_coche + TL)) command(id_coche,TL);
        else command(id_coche,None);
      }
    }
  }
  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
      guerreros();
      coches();
  }

};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
