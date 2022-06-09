# include "AIPlayer.h"
# include "Parchis.h"

const double masinf = 9999999999.0, menosinf = -9999999999.0;
const double gana = masinf - 1, pierde = menosinf + 1;
const int num_pieces = 4;
const int PROFUNDIDAD_MINIMAX = 4;  // Umbral maximo de profundidad para el metodo MiniMax
const int PROFUNDIDAD_ALFABETA = 6; // Umbral maximo de profundidad para la poda Alfa_Beta

bool AIPlayer::move(){
    cout << "Realizo un movimiento automatico" << endl;
    
    color c_piece;
    int id_piece;
    int dice;
    think(c_piece, id_piece, dice);

    cout << "\nMovimiento elegido: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    actual->movePiece(c_piece, id_piece, dice);
    return true;
}

void AIPlayer::think(color & c_piece, int & id_piece, int & dice) const{
    switch(id){
        //elegimos el comportamiento del agente
        case 0:
            thinkAleatorio(c_piece,id_piece,dice);
            break;
        case 1:
            thinkAleatorioMasInteligente(c_piece,id_piece,dice);
            break;
        case 2:
            thinkFichaMasAdelantada(c_piece,id_piece,dice);
            break;
        case 3:
            thinkMejorOpcion(c_piece,id_piece,dice);
            break;
        case 4:
            cout<<"\nPODA ALFA BETA";
            thinkMayorBeneficio(c_piece,id_piece,dice);
            break;
    }
}

void AIPlayer::thinkAleatorio(color &c_piece, int &id_piece, int &dice) const{
    // IMPLEMENTACIÓN INICIAL DEL AGENTE
    // Esta implementación realiza un movimiento aleatorio.
    // Se proporciona como ejemplo, pero se debe cambiar por una que realice un movimiento inteligente 
    //como lo que se muestran al final de la función.
    
    // OBJETIVO: Asignar a las variables c_piece, id_piece, dice (pasadas por referencia) los valores, 
    //respectivamente, de:
    // - color de ficha a mover
    // - identificador de la ficha que se va a mover
    // - valor del dado con el que se va a mover la ficha.

    // El color de ficha que se va a mover
    c_piece = actual->getCurrentColor();

    // Vector que almacenará los dados que se pueden usar para el movimiento
    vector<int> current_dices;
    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
    vector<int> current_pieces;

    // Se obtiene el vector de dados que se pueden usar para el movimiento
    current_dices = actual->getAvailableDices(c_piece);
    // Elijo un dado de forma aleatoria.
    dice = current_dices[rand() % current_dices.size()];

    // Se obtiene el vector de fichas que se pueden mover para el dado elegido
    current_pieces = actual->getAvailablePieces(c_piece, dice);

    // Si tengo fichas para el dado elegido muevo una al azar.
    if(current_pieces.size() > 0){
        id_piece = current_pieces[rand() % current_pieces.size()];
    }
    else{
        // Si no tengo fichas para el dado elegido, pasa turno (la macro SKIP_TURN me permite no mover).
        id_piece = SKIP_TURN;
    }

    /*
    // El siguiente código se proporciona como sugerencia para iniciar la implementación del agente.

    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha = menosinf, beta = masinf; // Cotas iniciales de la poda AlfaBeta
    // Llamada a la función para la poda (los parámetros son solo una sugerencia, se pueden modificar).
    valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
    cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    // ----------------------------------------------------------------- //

    // Si quiero poder manejar varias heurísticas, puedo usar la variable id del agente para usar una u otra.
    switch(id){
        case 0:
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
            break;
        case 1:
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion1);
            break;
        case 2:
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion2);
            break;
    }
    cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    */
}

void AIPlayer::thinkAleatorioMasInteligente(color &c_piece, int &id_piece, int &dice) const{
    //El color de la ficha que se va a mover
    c_piece=actual->getCurrentColor();

    //Vector que almacenará los dados que se pueden usar para el movimiento
    vector<int> current_dices;
    //Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido
    vector<int> current_pieces;

    //Se obtiene el vector de dados que se pueden usar para el movimiento
    current_dices=actual->getAvailableDices(c_piece);

    // En vez de elegir un dado al azar, miro primero cuales tienen fichas que se puedan mover
    vector<int> current_dices_que_pueden_mover_ficha;
    for(int i=0;i<current_dices.size();i++){
        //Se obtiene el vector de fichas que se pueden mover para el dado elegido
        current_pieces=actual->getAvailablePieces(c_piece,current_dices[i]);

        //Si se pueden mover fichas para el dado actual, lo añado al vector de dados que pueden mover fichas
        if(current_pieces.size()>0){
            current_dices_que_pueden_mover_ficha.push_back(current_dices[i]);
        }
    }

    //Si no tengo ningún dado que pueda mover fichas, paso turno con un dado al azar (la macro SKIP_TURN me permite no mover)-
    if(current_dices_que_pueden_mover_ficha.size()==0){
        dice=current_dices[rand() % current_dices.size()];

        id_piece=SKIP_TURN;
    }
    //En caso contrario, elijo un dado de forma aleatoria de entre los que pueden mover ficha
    else{
        dice=current_dices_que_pueden_mover_ficha[rand() % current_dices_que_pueden_mover_ficha.size()];

        //Se obtiene el evecto de fichas que se pueden mover para el dado elegido
        current_pieces=actual->getAvailablePieces(c_piece,dice);

        //Muevo un ficha al azar de entre las que se pueden mover
        id_piece=current_pieces[rand() % current_pieces.size()];
    }

}

void AIPlayer::thinkFichaMasAdelantada(color &c_piece, int &id_piece, int &dice) const{
    //Elijo el dado haciendo lo mismo que el jugador anterior
    thinkAleatorioMasInteligente(c_piece,id_piece,dice);
    //Tras llamar a esta función, ya tengo en dice el numero de dado que quiero usar
    //Ahora en vez de mover una ficha al azar, voy a mover la que esté mas adelantada
    // (equivalentemente, la más cercana a la meta).

    vector<int> current_pieces=actual->getAvailablePieces(c_piece,dice);

    int id_ficha_mas_adelantada=-1;
    int min_distancia_meta=9999;
    for(int i=0;i<current_pieces.size();i++){
        //distanciaToGoal(color, id) devuelve la distancia a la meta de la ficha [id] del color que le indique
        int distancia_meta=actual->distanceToGoal(c_piece,current_pieces[i]);
        if(distancia_meta< min_distancia_meta){
            min_distancia_meta=distancia_meta;
            id_ficha_mas_adelantada=current_pieces[i];
        }
    }

    //Si no he encontrado ninguna ficha, paso turno
    if(id_ficha_mas_adelantada == -1){
        id_piece=SKIP_TURN;
    }
    //En caso contrario, movere la ficha mas adelantada
    else{
        id_piece=id_ficha_mas_adelantada;
    }

}

void AIPlayer::thinkMejorOpcion(color &c_piece, int &id_piece, int &dice) const{
    //Vamos a mirar todos los posibles moviminetos del jugador actual accediendo a los hijos del estado actual
    //generateNextMove va iterando sobre cada hijo. Le paso la acción del último movimiento sobre el que he iterado
    // y me devolverá el siguiente. Inicialmente, cuando aún no he hecho ningún
    // movimiento, lo inicializo así.
    color last_c_piece=none; //El color de la ultima ficha que se movió
    int last_id_piece=-1;
    int last_dice=-1;

    //Cuando ya he recorrido todos los hijos, la funcion devuelve el estado actual. De esta forma puedo saber cuando paro de iterar

    Parchis siguiente_hijo=actual->generateNextMove(last_c_piece,last_id_piece,last_dice);

    bool me_quedo_con_esta_accion=false;

    while(!(siguiente_hijo==*actual) && !me_quedo_con_esta_accion){
        //comprobamos si el movimiento que nos permite llegar a ese hijo se corresponde con alguna de las situaciones especiales
        if(siguiente_hijo.isEatingMove() or siguiente_hijo.isGoalMove() or (siguiente_hijo.gameOver() and siguiente_hijo.getWinner()==this->jugador)){
            //me quedo con la accion actual, actualizandose las tres variables para llegar a dicho hijo
            me_quedo_con_esta_accion=true;

        }
        else{
            //genero el siguiente hijo
            siguiente_hijo=actual->generateNextMove(last_c_piece,last_id_piece,last_dice);
        }
    }

    //Si he encontrado una accion que me interesa, la guardo en las variables pasadas por referencia
    if(me_quedo_con_esta_accion){
        c_piece=last_c_piece;
        id_piece=last_id_piece;
        dice=last_dice;
    }
    //Si no, muevo la ficha mas adelantada como antes
    else{
        thinkFichaMasAdelantada(c_piece,id_piece,dice);
    }
}

double AIPlayer::ValoracionTest(const Parchis &estado, int jugador)
{
    // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.


    int ganador = estado.getWinner();
    int oponente = (jugador + 1) % 2;

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                // Valoro positivamente que la ficha esté en casilla segura o meta.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_jugador++;
                }
                else if (estado.getBoard().getPiece(c, j).type == goal)
                {
                    puntuacion_jugador += 5;
                }
            }
        }

        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                if (estado.isSafePiece(c, j))
                {
                    // Valoro negativamente que la ficha esté en casilla segura o meta.
                    puntuacion_oponente++;
                }
                else if (estado.getBoard().getPiece(c, j).type == goal)
                {
                    puntuacion_oponente += 5;
                }
            }
        }

        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}

double AIPlayer::MiValoracion(const Parchis &estado, int jugador){
    //defino el id de mi oponente
    int oponente=(jugador+1)%2;
    //primero compruebo si hay o no ganador para dicho estado de juego
    int ganador=estado.getWinner();
    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else{
        //no hay ganador, seguimos valorando
        int puntuacion_jugador=0;
        int puntuacion_oponente=0;
        //comprobamos si se ha comido alguna ficha en el ultimo movimiento
        color color_comido=get<0>(estado.eatenPiece()); //color de la ficha comida
        vector<color> my_colors = estado.getPlayerColors(jugador); //mis colores
        vector<color> op_colors = estado.getPlayerColors(oponente);

        if(estado.isEatingMove()){
            if(color_comido==my_colors[0] or color_comido==my_colors[1]){
                //si la ficha comida es una de las fichas de 'jugador', esto lo perjudica
                puntuacion_oponente++;
            }
            else{
                //nos hemos comido una ficha del oponente
                puntuacion_jugador++;
            }

        }

        //valoramos todas aquellas fichas situadas en casa,casilla segura,pasillo final o meta, y minizamos la distancia de cada ficha a la meta
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                // Valoro positivamente que la ficha esté en casilla segura o meta.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_jugador++;
                }
                else if (estado.getBoard().getPiece(c, j).type == goal)
                {
                    puntuacion_jugador += 5;
                }
                else if(estado.getBoard().getPiece(c, j).type == home){
                    //esto no me beneficia,
                    puntuacion_jugador--;
                }
                else if(estado.getBoard().getPiece(c,j).type == final_queue){
                    puntuacion_jugador+=3;
                }
                //intento conseguir que mis fichas esten cada vez mas cerca de la meta, por lo que minimizo esa distancia
                //distancia de la casilla a la meta
                puntuacion_jugador-=estado.distanceToGoal(c,j);
            }
        }

        //FICHAS DEL OPONENTE
        //cuantas mas fichas del oponente haya en zona de casa, con mayor libertad me podre desplazar por el tablero sin el riesgo de ser comido
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                // Valoro positivamente que la ficha esté en casilla segura o meta.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_oponente++;
                }
                else if (estado.getBoard().getPiece(c, j).type == goal)
                {
                    puntuacion_oponente += 5;
                }
                else if(estado.getBoard().getPiece(c, j).type == home){
                    //esto no me beneficia,
                    puntuacion_oponente--;
                }
                else if(estado.getBoard().getPiece(c,j).type == final_queue){
                    puntuacion_oponente+=3;
                }

                //distancia de la casilla a la meta
                puntuacion_oponente-=estado.distanceToGoal(c,j);
            }
        }


        //ahora gestiono las barreras (casilla donde se alojan dos fichas del mismo color)
        //si la barrera es mia, bloqueo al oponente y me favorece
        //si la barrera es del oponente, me bloquea el paso a mi y me perjudica
        
        //tomo el vector de colores de las barreras entre la primera y ultima casilla del tablero
        vector<color> color_barreras=estado.anyWall(Box(1, normal, none),Box(68, normal, none));
        if(!color_barreras.empty()){
            //si hay alguna barrera, recorro el vector comprobando el color de la barrera
            for(int i=0;i<color_barreras.size();i++){
                if(color_barreras[i]==my_colors[0] or color_barreras[i]==my_colors[1]){
                    //mi barrera
                    puntuacion_jugador++;
                }
                else{
                    //barrera del oponente
                    puntuacion_oponente++;
                }
            }
        }

        return (puntuacion_jugador-puntuacion_oponente);
    }

}

double AIPlayer::Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, double alpha, double beta, double (*heuristic)(const Parchis &, int)) const{
    //Implementacion del algoritmo de busqueda con adversario Poda Alpha-Beta
    //comprobamos si el estado actual representa un nodo terminal (hay un ganador) o si hemos alcanzado la profundidad maxima permitida
    if(profundidad==profundidad_max or actual.gameOver()){
        //utilizamos la heuristica para asignar una valoracion en funcion del estado de la partida
        //usamos de momento la definida por defecto
        return ValoracionTest(actual,jugador);
    }
    
    double aux;
    //generamos el primer hijo
    color last_c_piece=none;
    int last_id_piece=-1;
    int last_dice=-1;

    //definimos variables auxiliares para evitar que los hijos modifiquen el mejor movimiento del padre
    color aux_c_piece=none;
    int aux_id_piece=-1;
    int aux_dice=-1;

    

    Parchis siguiente_hijo=actual.generateNextMoveDescending(last_c_piece,last_id_piece,last_dice);
    //nos quedamos con el movimiento que nos lleva al primer hijo para no perderlo
    //juanardo le pasa c_piece, idpiece y dice al generar el primer hijo, e igualaba en el if a jugador

    //comprobamos si el nodo padre es de MAX o de MIN, para saber si debemos usar 'alpha' o 'beta'
    if(actual.getCurrentPlayerId()==0){
        //cout<<"\nPrimer jugador";
        //Jugador 0 ('MAX')
        //hallamos 'alpha' para los hijos
        while(!(siguiente_hijo==actual)){
            //la poda alfa_beta se aplica sobre el nuevo tablero hijo, no sobre el padre ('actual')
            //alpha=max(alpha,Poda_AlfaBeta(siguiente_hijo,siguiente_hijo.getCurrentPlayerId(),profundidad+1,profundidad_max,c_piece,id_piece,dice,alpha,beta,ValoracionTest));
            aux=Poda_AlfaBeta(siguiente_hijo,jugador,profundidad+1,profundidad_max,aux_c_piece,aux_id_piece,aux_dice,alpha,beta,ValoracionTest);
            //comprobamos si me ofrece una mejor valoracion a la que aspiro actualemente
            if(aux>alpha){
                // de momento me interesa ese hijo, asi que guardo el camino que me lleva a el y la valoracion que me ofrece
                alpha=aux;
                c_piece=last_c_piece;
                id_piece=last_id_piece;
                dice=last_dice;
            }
            if(alpha>=beta){
                //cout<<"\nPoda beta";
                //break; //poda beta
                return alpha; 
                //dejamos de explorar
            }
            //generamos el siguiente hijo a partir del tablero padre 'actual' y hallamos su valoracion
            //siguiente_hijo=actual.generateNextMoveDescending(c_piece,id_piece,dice);
            siguiente_hijo=actual.generateNextMoveDescending(last_c_piece,last_id_piece,last_dice);
        }
        return alpha;
        
    }
    else{
        //el nodo padre es un nodo MIN, tomamos la valoracion del hijo que menos nos beneficie
        while(!(siguiente_hijo==actual)){
            //beta=min(alpha,Poda_AlfaBeta(siguiente_hijo,siguiente_hijo.getCurrentPlayerId(),profundidad+1,profundidad_max,c_piece,id_piece,dice,alpha,beta,ValoracionTest));
            //hallamos el beneficio que me aporta ese hijo y lo comparamos con el que beneficio al que aspiramos actualmente
            aux=Poda_AlfaBeta(siguiente_hijo,jugador,profundidad+1,profundidad_max,aux_c_piece,aux_id_piece,aux_dice,alpha,beta,ValoracionTest);
            if(aux<beta){
                //a MIN le interesa ese hijo ya que ofrece a MAX un beneficio menor aun del que ya puede aspirar
                beta=aux;
                //guardamos el movimiento que nos lleva al hijo que de momento menor beneficio asegura a MAX
                c_piece=last_c_piece;
                id_piece=last_id_piece;
                dice=last_dice;
            }
            if(beta<=alpha){
                //break; //poda beta
                //dejamos de explorar ese hijo
                return beta;
            }
            //generamos el siguiente hijo y hallamos su valoracion
            siguiente_hijo=actual.generateNextMoveDescending(last_c_piece,last_id_piece,last_dice);
        }
        return beta;
    }

}

void AIPlayer::thinkMayorBeneficio(color &c_piece, int &id_piece, int &dice) const{
    //cota inferior
    double alpha=menosinf;
    //cota superior
    double beta=masinf;
    //invocamos a poda alpha-beta para obtener el movimiento que nos lleva al hijo que mas nos beneficie
    //const Parchis tablero_actual=*actual;

    //c_piece=actual->getCurrentColor();
    cout<<"\nSoy el jugador "<<actual->getCurrentPlayerId();
    //double valoracion=Poda_AlfaBeta(tablero_actual,tablero_actual.getCurrentPlayerId(),0,PROFUNDIDAD_ALFABETA,c_piece,id_piece,dice,alpha,beta,ValoracionTest);
    double valoracion = Poda_AlfaBeta(*actual, actual->getCurrentPlayerId(),0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion);
    //se habran ajustado las variables c_piece, id_piece y dice al movimiento que nos lleva al mejor hijo...
    
}

