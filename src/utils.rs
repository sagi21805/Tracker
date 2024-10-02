use num::NumCast;

pub fn interpolate<T>(a: T, b: T, ratio: f32) -> T
where
    T: NumCast + Copy,
{
    let a_f32 = T::to_f32(&a).expect("can't convert into float");
    let b_f32 = T::to_f32(&b).expect("can't convert into float");
    let result_f32 = a_f32 * ratio + b_f32 * (1.0 - ratio);

    T::from(result_f32).expect("can't convert into the original type") // Convert the result back to type T
}
