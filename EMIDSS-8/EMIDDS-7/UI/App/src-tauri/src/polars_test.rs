use polars::prelude::*;
use serde_json::json;

fn main() {
    let times = vec!["11:13".to_string(), "11:14".to_string(), "11:15".to_string()];
    let temps = vec![Some(27.0), None, Some(28.0)];
    
    let time_s = Series::new("Time".into(), times);
    let temp_s = Series::new("Temperature".into(), temps);
    
    let df = DataFrame::new(vec![time_s.into(), temp_s.into()]).unwrap();
    
    let lf = df.lazy().with_columns([
        col("Temperature").interpolate(InterpolationMethod::Linear)
    ]);
    
    let mut clean_df = lf.collect().unwrap();
    
    let times_col = clean_df.column("Time").unwrap().str().unwrap();
    let temps_col = clean_df.column("Temperature").unwrap().f64().unwrap();
    
    let mut json_data = Vec::new();
    for i in 0..clean_df.height() {
        json_data.push(json!({
            "Time": times_col.get(i).unwrap_or(""),
            "Temperature": temps_col.get(i).unwrap_or(0.0)
        }));
    }
    
    println!("{:?}", json_data);
}
