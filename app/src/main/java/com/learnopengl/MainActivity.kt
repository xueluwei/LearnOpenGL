package com.learnopengl

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.learnopengl.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        binding.sampleText.text = "${stringFromJNI()} ${stringFromMainJNI()}"
    }

    /**
     * A native method that is implemented by the 'learncpp' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String
    external fun stringFromMainJNI(): String

    companion object {
        // Used to load the 'learncpp' library on application startup.
        init {
            System.loadLibrary("learncpp")
            System.loadLibrary("main")
        }
    }
}