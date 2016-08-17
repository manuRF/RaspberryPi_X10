
package rpi.casa.domotica;
/*   Created by Manu on 27/02/2016.
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
*/
/*
 *  Cliente Android de conexion mediante Sockets TCP a un servidor (python-RPi)
 *  RPi maneja en el GPIO_0 un emisor de RF (433Khz) que por protocolo X10
 *  conecta a varios modulos ya instalados anteriormente en la sala de mi casa
 *  para controlar cuatro luces y dos persianas.
 */


        import java.util.concurrent.ExecutionException;

        import android.annotation.SuppressLint;
        import android.content.Intent;
        import android.content.SharedPreferences;
        import android.os.Bundle;
        import android.preference.PreferenceManager;
        import android.support.v7.app.ActionBarActivity;
        import android.support.v7.app.AppCompatActivity;
        import android.view.Menu;
        import android.view.MenuItem;
        import android.view.MotionEvent;
        import android.view.View;
        import android.widget.Button;
        import android.widget.Toast;
        import android.widget.ToggleButton;
/*
#define A1ON    "0110000000000000"
#define A1OFF   "0110000000100000"
#define A2ON    "0110000000010000"
#define A2OFF   "0110000000110000"
#define A3ON    "0110000000001000"
#define A3OFF   "0110000000101000"
#define ABRIGHT "0110000010001000"
#define ADIM    "0110000010011000"
*/

public class MainActivity extends AppCompatActivity {
    /*
     *  Variables globales de la Actividad principal
     */
    private String host;
    private String PORT;
    private static final int RESULT_SETTINGS = 1;
    private ToggleButton BLucesSofa = null;
    private ToggleButton BLucesEntrada = null;
    private ToggleButton BLucesMueble = null;
    private ToggleButton BLucesTele = null;
    private Button BBajaPersianaIzquierda = null;
    private Button BSubePersianaIzquierda = null;
    private Button BBajaPersianaDerecha = null;
    private Button BSubePersianaDerecha = null;
    /*
     * (non-Javadoc)
     * @see android.support.v7.app.ActionBarActivity#onCreate(android.os.Bundle)
     *  Crea la ventana y la Actividad princiapl
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        SharedPreferences sharedPrefs = PreferenceManager.getDefaultSharedPreferences(this);
        host = sharedPrefs.getString("prefHostIP", "NULL");
        PORT = sharedPrefs.getString("prefHostPort", "NULL");
/*
 * Pulsador "SUBIR PERSIANA DERECHA"
 */

        BSubePersianaDerecha = (Button) findViewById(R.id.SubePersianaDerecha);
        BSubePersianaDerecha.setOnTouchListener(new View.OnTouchListener() {
            @SuppressLint("ClickableViewAccessibility")
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case (MotionEvent.ACTION_DOWN):
                        BBajaPersianaDerecha.setEnabled(false);
                    //    SendCommand("A3ON    ");
                        SendCommand("0110000000001000");
                        break;
                    case (MotionEvent.ACTION_UP):
                    //    SendCommand("ABRIGHT ");
                        SendCommand("0110000010001000");
                        BBajaPersianaDerecha.setEnabled(true);
                        break;
                }
                return true;
            }
        });
/*
 * Pulsador "BAJAR PERSIANA DERECHA"
 */
        BBajaPersianaDerecha = (Button) findViewById(R.id.BajaPersianaDerecha);
        BBajaPersianaDerecha.setOnTouchListener(new View.OnTouchListener() {
            @SuppressLint("ClickableViewAccessibility")
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case (MotionEvent.ACTION_DOWN):
                        BSubePersianaDerecha.setEnabled(false);
                    //    SendCommand("A3OFF   ");
                        SendCommand("0110000000101000");
                        break;
                    case (MotionEvent.ACTION_UP):
                    //    SendCommand("ADIM    ");
                        SendCommand("0110000010011000");
                        BSubePersianaDerecha.setEnabled(true);
                        break;
                }
                return true;
            }
        });

/*
 *  Pulsador "SUBIR PERSIANA IZQUIERDA"
 */
        BSubePersianaIzquierda = (Button) findViewById(R.id.SubePersianaIzquierda);
        BSubePersianaIzquierda.setOnTouchListener(new View.OnTouchListener() {
            @SuppressLint("ClickableViewAccessibility")
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case (MotionEvent.ACTION_DOWN):
                        BBajaPersianaIzquierda.setEnabled(false);
                    //    SendCommand("A2ON    ");
                        SendCommand("0110000000010000");
                        break;

                    case (MotionEvent.ACTION_UP):
                    //    SendCommand("ABRIGHT ");
                        SendCommand("0110000010001000");
                        BBajaPersianaIzquierda.setEnabled(true);
                        break;
                }
                return true;
            }
        });
/*
 * Pulsador "BAJAR PERSIANA IZQUIERDA"
 */
        BBajaPersianaIzquierda = (Button) findViewById(R.id.BajaPersianaIzquierda);
        BBajaPersianaIzquierda.setOnTouchListener(new View.OnTouchListener() {
            @SuppressLint("ClickableViewAccessibility")
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case (MotionEvent.ACTION_DOWN):
                        BSubePersianaIzquierda.setEnabled(false);
                    //SendCommand("A2OFF   ");
                        SendCommand("0110000000110000");
                        break;
                    case (MotionEvent.ACTION_UP):
                    //    SendCommand("ADIM    ");
                        SendCommand("0110000010011000");
                        BSubePersianaIzquierda.setEnabled(true);
                        break;
                }
                return true;
            }
        });
/*
 * Lampara "LUZ SOFA"
 */
        BLucesSofa = (ToggleButton) findViewById(R.id.LucesSofa);
        //BLucesSofa.setEnabled(false);
        BLucesSofa.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (BLucesSofa.isChecked())
                     SendCommand("0110000000000000");   //("A1ON    ");
                else SendCommand("0110000000100000");   //("A1OFF   ");
            }
        });
/*
 *  Lampara "LUZ ENTRADA"
 */
        BLucesEntrada = (ToggleButton) findViewById(R.id.LucesEntrada);
        //BLucesEntrada.setEnabled(false);
        BLucesEntrada.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (BLucesEntrada.isChecked())
                    SendCommand("0110000000000000");   //("A1ON    ");
                else SendCommand("0110000000100000");   //("A1OFF   ");
            }
        });
/*
 * Lampara  "LUZ MUEBLE"
 *
 */
        BLucesMueble = (ToggleButton) findViewById(R.id.LucesMueble);
    //    BLucesMueble.setEnabled(false);
        BLucesMueble.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                if (BLucesMueble.isChecked())
                    SendCommand("0110000000000000");   //("A1ON    ");
                else SendCommand("0110000000100000");   //("A1OFF   ");
            }
        });
/*
 *  Lampara "LUZ TELE"
 */
        BLucesTele = (ToggleButton) findViewById(R.id.LucesTele);
    //    BLucesTele.setEnabled(false);
        BLucesTele.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (BLucesTele.isChecked())
                    SendCommand("0110000000000000");   //("A1ON    ");
                else SendCommand("0110000000100000");   //("A1OFF   ");
            }
        });
    }
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.O
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    /*
     * (non-Javadoc)
     * @see android.app.Activity#onOptionsItemSelected(android.view.MenuItem)
     */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        switch (id) {
            case (R.id.action_settings): {
                Intent settings = new Intent(this, UserSettings.class);
                startActivityForResult(settings, RESULT_SETTINGS);
            }
            break;
        }
        return true;
    }
    /*
     *  Envia los comandos X10 al servidor RPi.
     *  Si falla la conexion avisa con un mensage de ERROR
     */
    void SendCommand(String DEVICE) {
        RPiSocketConnection Send2RPi = new RPiSocketConnection();
        try {
            if (Send2RPi.execute(host,PORT, DEVICE).get() == "ERROR")
                Toast.makeText(this, "ERROR de conexion, revisa los ajustes ", Toast.LENGTH_LONG).show();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (ExecutionException e) {
            e.printStackTrace();
        }
    }
}
/*
 *
 *
 */
