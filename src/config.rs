use std::fs::File;
use std::io::Read;
use serde_json::{Value, Error as JsonError};
use std::sync::{Arc, RwLock};

// Define a configuration struct that holds all necessary values
#[derive(Debug, Default)]
pub struct Config {
    pub elements_in_point: u8,
    pub starting_id: usize,
    pub vel_dead_band: usize,
    pub min_score: f32,
    pub move_to_last_seen: u8,
    pub min_confidence: f32,
    pub to_visualize: bool,
    pub wait_key: u16,
    pub colors: Vec<u8>, // Adjust type according to your actual needs
    pub size_coefficient: f32,
    pub smoothing_frames: u8,
    pub vel_coefficient: f32,
}

// Shared state using Arc and RwLock for safe access across threads
pub struct SharedState {
    pub config: Arc<RwLock<Config>>,
}

impl SharedState {
    pub fn new() -> Self {
        SharedState {
            config: Arc::new(RwLock::new(Config::default())),
        }
    }
}

fn get_rgb_array(array: &Value, key: &str) -> Result<u8, String> {
    if array.as_array().map_or(false, |a| a.len() == 3) {
        let rgb = array.as_array().unwrap();
        for i in 0..3 {
            if !rgb[i].is_u64() || rgb[i].as_u64().unwrap() > 255 {
                return Err(format!("Invalid RGB array format in key \"{}\". Values are too big.", key));
            }
        }
        // Adjust this according to your actual needs
        Ok(rgb[2].as_u64().unwrap() as u8) // Placeholder
    } else {
        Err(format!("Invalid RGB array format in key \"{}\". The array is too long.", key))
    }
}

fn get_rgb_vector(j: &Value, key: &str) -> Result<Vec<u8>, String> {
    let mut rgb_vector = Vec::new();
    if let Some(obj) = j.as_object() {
        for (k, v) in obj {
            if v.is_array() {
                rgb_vector.push(get_rgb_array(v, k)?);
            } else {
                return Err(format!("Key '{}' not found or not an array in the JSON object.", k));
            }
        }
    }
    Ok(rgb_vector)
}

fn load_configuration(config: &serde_json::Value, state: &SharedState) -> Result<(), String> {
    let elements_in_point = config["elementsInPoint"]
        .as_u64()
        .ok_or("'elementsInPoint' is not an unsigned number.")? as u8;
    let starting_id = config["startingId"]
        .as_u64()
        .ok_or("'startingId' is not an unsigned number.")? as usize;
    let velocity_dead_band = config["velocityDeadBand"]
        .as_u64()
        .ok_or("'velocityDeadBand' is not an unsigned number.")? as usize;
    let min_score = config["minScore"]
        .as_f64()
        .ok_or("'minScore' is not a float OR greater than 1")? as f32;
    if min_score > 1.0 {
        return Err("'minScore' is greater than 1".into());
    }
    let move_to_last_seen = config["moveToLastSeen"]
        .as_u64()
        .ok_or("'moveToLastSeen' is not an unsigned number")? as u8;
    let min_confidence = config["minConfidence"]
        .as_f64()
        .ok_or("'minConfidence' is not an unsigned number OR greater than 1")? as f32;
    if min_confidence >= 1.0 {
        return Err("'minConfidence' is greater than or equal to 1".into());
    }
    
    // Predictions
    let size_coefficient = config["predictions"]["sizeCoefficient"]
        .as_f64()
        .ok_or("'sizeCoefficient' is not a float.")? as f32;
    if size_coefficient <= 0.0 {
        return Err("'sizeCoefficient' is not positive".into());
    }
    let velocity_coefficient = config["predictions"]["velocityCoefficient"]
        .as_f64()
        .ok_or("'velocityCoefficient' is not a float.")? as f32;
    if velocity_coefficient <= 0.0 {
        return Err("'velocityCoefficient' is not positive".into());
    }
    
    // Visualization
    let to_visualize = config["visualization"]["toVisualize"]
        .as_bool()
        .ok_or("'toVisualize' is not a boolean.")?;
    let wait_key = config["visualization"]["waitKey"]
        .as_u64()
        .ok_or("'waitKey' is not an unsigned number.")? as u16;
    let colors = get_rgb_vector(&config["visualization"]["colors"], "colors")?;
    
    // Update the shared config state safely
    {
        let mut config = state.config.write().unwrap();
        config.elements_in_point = elements_in_point;
        config.starting_id = starting_id;
        config.vel_dead_band = velocity_dead_band;
        config.min_score = min_score;
        config.move_to_last_seen = move_to_last_seen;
        config.min_confidence = min_confidence;
        config.size_coefficient = size_coefficient;
        config.smoothing_frames = 2; // Assuming this is a constant
        config.vel_coefficient = velocity_coefficient;
        config.to_visualize = to_visualize;
        config.wait_key = wait_key;
        config.colors = colors;
    }

    Ok(())
}

