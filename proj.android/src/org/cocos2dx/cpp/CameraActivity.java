package org.cocos2dx.cpp;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.hardware.Camera;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.media.Image;
import android.media.MediaScannerConnection;
import android.media.MediaScannerConnection.MediaScannerConnectionClient;
import android.view.Display;
import android.graphics.PointF;
import android.graphics.drawable.Drawable;
import android.app.ProgressDialog;
import android.view.KeyEvent;
import android.os.AsyncTask;
import android.util.Log;
import java.util.Calendar;
import android.content.DialogInterface;
import com.IHMTEK.LudoMuse.R;


import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.widget.Toast;
import android.widget.ImageButton;
import android.widget.ImageView;

import java.io.Console;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.lang.Override;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URL;

import org.cocos2dx.cpp.jniFacade.JniCppFacade;


public class CameraActivity extends Activity implements OnClickListener {

    private SurfaceView preview = null;
    private SurfaceHolder previewHolder = null;
    private ImageButton nextButton = null;
    private ImageView imageView = null;
    
    private Camera camera = null;
    private boolean inPreview = false;
    ImageView image;
    Bitmap bmp, itembmp;
    static Bitmap mutableBitmap;
    PointF start = new PointF();
    PointF mid = new PointF();
    float oldDist = 1f;
    File imageFileName = null;
    File imageFileFolder = null;
    private MediaScannerConnection msConn;
    Display d;
    int screenhgt, screenwdh;
    ProgressDialog dialog;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.cameraview);

        //image = (ImageView) findViewById(R.id.image);
        preview = (SurfaceView) findViewById(R.id.surface);

        previewHolder = preview.getHolder();
        previewHolder.addCallback(surfaceCallback);
        previewHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);

        previewHolder.setFixedSize(getWindow().getWindowManager()
            .getDefaultDisplay().getWidth(), getWindow().getWindowManager()
            .getDefaultDisplay().getHeight());


        nextButton = (ImageButton) findViewById(R.id.validateButton);
        nextButton.setBackgroundColor(Color.TRANSPARENT);
        nextButton.setPadding(0, 0, 0, 0);
        
        nextButton.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
//				finishActivity(1);
				camera.takePicture(null, null, photoCallback);
//				finish();
			}
		});
        
        String maskPath = getIntent().getStringExtra("maskPath");
        try {
    		URL url = getClass().getResource("/assets/" + maskPath);
    		
//    		String path = getFilesDir().getAbsolutePath() + "/assets/" + maskPath;
//			URL url = new URL(path);
			Bitmap bmp = BitmapFactory.decodeStream(url.openConnection().getInputStream());
	        imageView = (ImageView) findViewById(R.id.imageView);
	        imageView.setImageBitmap(bmp);
		} catch (MalformedURLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			System.out.println("unable to read file : " + e.toString());
		}
        
    }

    @Override
    public void onClick(DialogInterface a_oTnterface, int i) {

    }

    @Override
    public void onResume() {
        super.onResume();
        camera = Camera.open();
    }

    @Override
    public void onPause() {
        if (inPreview) {
            camera.stopPreview();
        }

        camera.release();
        camera = null;
        inPreview = false;
        super.onPause();
    }

    private Camera.Size getBestPreviewSize(int width, int height, Camera.Parameters parameters) {
        Camera.Size result = null;
        for (Camera.Size size: parameters.getSupportedPreviewSizes()) {
            if (size.width <= width && size.height <= height) {
                if (result == null) {
                    result = size;
                } else {
                    int resultArea = result.width * result.height;
                    int newArea = size.width * size.height;
                    if (newArea > resultArea) {
                        result = size;
                    }
                }
            }
        }
        return (result);
    }

    SurfaceHolder.Callback surfaceCallback = new SurfaceHolder.Callback() {
        public void surfaceCreated(SurfaceHolder holder) {
            try {
                camera.setPreviewDisplay(previewHolder);
            } catch (Throwable t) {
                Log.e("PreviewDemo-surfaceCallback",
                    "Exception in setPreviewDisplay()", t);
                Toast.makeText(CameraActivity.this, t.getMessage(), Toast.LENGTH_LONG)
                    .show();
            }
        }

        public void surfaceChanged(SurfaceHolder holder,
        int format, int width,
        int height) {
            Camera.Parameters parameters = camera.getParameters();
            Camera.Size size = getBestPreviewSize(width, height,
            parameters);

            if (size != null) {
                parameters.setPreviewSize(size.width, size.height);
                camera.setParameters(parameters);
                camera.startPreview();
                inPreview = true;
            }
        }

        public void surfaceDestroyed(SurfaceHolder holder) {
            // no-op
        }
    };


    Camera.PictureCallback photoCallback = new Camera.PictureCallback() {
        public void onPictureTaken(final byte[] data, final Camera camera) {
            dialog = ProgressDialog.show(CameraActivity.this, "", "Saving Photo");
            new Thread() {
                public void run() {
                    try{
                        sleep(1000);
                    }
                    catch(Exception e) {

                    }
                    onPictureTake(data, camera);
                }
            }.start();

        }
    };



    public void onPictureTake(byte[] data, Camera camera) {
        bmp = BitmapFactory.decodeByteArray(data, 0, data.length);
        mutableBitmap = bmp.copy(Bitmap.Config.ARGB_8888, true);
        savePhoto(mutableBitmap);
        dialog.dismiss();
        finish();
    }



    class SavePhotoTask extends AsyncTask < byte[], String, String > {@Override
        protected String doInBackground(byte[]...jpeg) {
            File folder = new File(Environment.getExternalStorageDirectory(), "Cern");
            if (!folder.exists()) {
                if(!folder.mkdirs()){
                    Log.d("MyCameraApp", "failed to create directory");
                }
            }
            try {
                FileOutputStream fos = new FileOutputStream(folder.getPath());
                fos.write(jpeg[0]);
                fos.close();
            } catch (java.io.IOException e) {
                Log.e("PictureDemo", "Exception in photoCallback", e);
            }
            return (null);
        }
    }


    public void savePhoto(Bitmap bmp) {
        imageFileFolder = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES), "Cern");
        imageFileFolder.mkdir();
        FileOutputStream out = null;
        Calendar c = Calendar.getInstance();
        String date = fromInt(c.get(Calendar.MONTH)) + fromInt(c.get(Calendar.DAY_OF_MONTH)) + fromInt(c.get(Calendar.YEAR)) + fromInt(c.get(Calendar.HOUR_OF_DAY)) + fromInt(c.get(Calendar.MINUTE)) + fromInt(c.get(Calendar.SECOND));
        imageFileName = new File(imageFileFolder.getPath() + File.separator + date + ".jpg");
        try {
            imageFileName.createNewFile();
            out = new FileOutputStream(imageFileName);
            bmp.compress(Bitmap.CompressFormat.JPEG, 100, out);
            out.flush();
            out.close();
//            scanPhoto(imageFileName.toString());
            out = null;
        } catch (Exception e) {
            e.printStackTrace();
        }
        Intent intent = getIntent();
        intent.putExtra("imageName", imageFileName.getPath());
        JniCppFacade.setCurrentPicturePath(imageFileName.getPath());
    }

    public String fromInt(int val) {
        return String.valueOf(val);
    }

    public void scanPhoto(final String imageFileName) {
        msConn = new MediaScannerConnection(CameraActivity.this, new MediaScannerConnectionClient() {
            public void onMediaScannerConnected() {
                msConn.scanFile(imageFileName, null);
                Log.i("msClient obj  in Photo Utility", "connection established");
            }
            public void onScanCompleted(String path, Uri uri) {
                msConn.disconnect();
                Log.i("msClient obj in Photo Utility", "scan completed");
            }
        });
        msConn.connect();
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_MENU && event.getRepeatCount() == 0) {
            onBack();
        }
        return super.onKeyDown(keyCode, event);
    }

    public void onBack() {
        Log.e("onBack :", "yes");
        camera.takePicture(null, null, photoCallback);
        inPreview = false;
    }

}