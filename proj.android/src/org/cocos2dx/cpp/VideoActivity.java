package org.cocos2dx.cpp;

import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ImageButton;
import android.widget.VideoView;

import com.IHMTEK.LudoMuse.R;

/**
 * Created by IHMTEK Vibox 2 on 24/10/2016.
 */

public class VideoActivity extends Activity implements View.OnClickListener {



    private VideoView videoView = null;
    private ImageButton nextButton = null;


    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        Log.d("LudoMuse", "VideoActivity.onCreate");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.videoview);

        videoView = (VideoView) findViewById(R.id.videoView);

        nextButton = (ImageButton) findViewById(R.id.validateButton);
        nextButton.setBackgroundColor(Color.TRANSPARENT);
        nextButton.setPadding(0, 0, 0 ,0);

        nextButton.setOnClickListener(this);

        String videoPath = getIntent().getStringExtra("videoPath");
        Log.d("LudoMuse", "trying to play video file : " + videoPath);
        videoView.setVideoPath(videoPath);
        videoView.start();

    }

    @Override
    public void onClick(View v) {
        videoView.stopPlayback();
        finish();
    }

    @Override
    public void onPause()
    {
        videoView.pause();
        super.onPause();
    }

    @Override
    public void onResume()
    {
        super.onResume();
        videoView.start();
    }
}
