
package rpi.casa.domotica;

/**
 *   Created by Manu on 27/02/2016.
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
        import android.os.AsyncTask;
        import android.util.Log;

        import java.io.BufferedReader;
        import java.io.IOException;
        import java.io.InputStream;
        import java.io.InputStreamReader;
        import java.io.OutputStreamWriter;
        import java.io.PrintWriter;
        import java.net.InetSocketAddress;
        import java.net.MalformedURLException;
        import java.net.Socket;
        import java.net.UnknownHostException;

public class RPiSocketConnection extends AsyncTask<String, Integer, String> {
    protected String host;
    protected String PORT;
    protected String DEVICE;
    private static final String TAG = "RPiSocketConnection";

    protected void onPreExecute() {
        super.onPreExecute();
    }

    @Override
    protected String doInBackground(String... params) {
        this.host = params[0];
        this.PORT = params[1];
        this.DEVICE = params[2];
        Socket socket = null;
        String data = "";
        try {
            int port = Integer.parseInt(PORT);
            socket = new Socket();
            socket.connect(new InetSocketAddress(host,port), 1000);
            PrintWriter pw = new PrintWriter(new OutputStreamWriter(
                    socket.getOutputStream()), true);
            pw.println(DEVICE);

            data = readStream(socket.getInputStream());
            System.out.println(TAG+"==>Data:"+ data);

        } catch (UnknownHostException exception) {
            System.out.println(TAG+"==>ERROR UnknownHostException: "+exception);
            exception.printStackTrace();
            return "ERROR";
        } catch (IOException exception) {
            System.out.println(TAG+"==>ERROR IOException: " + exception);
            exception.printStackTrace();
            return "ERROR";
        } finally {
            if (null != socket)
                try {
                    socket.close();
                } catch (IOException e) {
                    System.out.println(TAG+"==>ERROR IOException: " +e); // Aqui falla la conexion.
                    Log.e(TAG, "IOException");
                    return"ERROR";
                }
        }
        return data;
    }
    @Override
    protected void onProgressUpdate(Integer... values) {
        super.onProgressUpdate(values);
    }
    @Override
    protected void onPostExecute(String result) {
        super.onPostExecute(result);

    }
    private String readStream(InputStream in) {
        BufferedReader reader = null;
        StringBuffer data = new StringBuffer();
        try {
            reader = new BufferedReader(new InputStreamReader(in));
            String line = "";
            while ((line = reader.readLine()) != null) {
                data.append(line);
            }
        } catch (IOException e) {
            Log.e(TAG, "IOException");
        } finally {
            if (reader != null) {
                try {
                    reader.close();
                } catch (IOException e) {
                    Log.e(TAG, "IOException");
                }
            }
        }
        return data.toString();
    }
}




