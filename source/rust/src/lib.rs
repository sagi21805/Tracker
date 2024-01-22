use std::os::raw::c_int;

#[allow(
    non_camel_case_types,
    non_upper_case_globals,
    non_snake_case
)]

#[no_mangle]
pub extern "C" fn printArray(ptr: *const c_int, len: usize){
    // Safety: We assume the pointer is valid for the specified length
    let slice: &[i32] = unsafe {
        std::slice::from_raw_parts(ptr as *const i32, len)
    };
    let rustVector: Vec<i32> = slice.to_vec();
    for item in rustVector{
        print!("{} ", item + 2);
    }
    println!("");
}

