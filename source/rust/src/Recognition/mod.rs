use rect::Rect;
mod rect;

#[repr(C)]
pub struct Recognition {
    pub rects: *mut Rect, 
    pub types: *mut u16,
    pub confidance: *mut u16,
    pub size: usize
}

impl Recognition {

    pub fn removeDups(&self, threshold: u16) {
        
        let confidances: &mut [u16] = unsafe { std::slice::from_raw_parts_mut(self.confidance, self.size) };
        let avgConf: u16 = confidances.iter().sum::<u16>() / self.size as u16; 
        let index = match confidances.binary_search_by(|&x| avgConf.cmp(&x)) {
            Ok(index) => index + 1,  // Exact match found
            Err(index) => index, // No exact match, index is where the target should be inserted (just how the binary search method works)
        };

        for conf in confidances.iter().skip(index) {

        }
        //TODO sort by intersection of union, if zero, its a standalone rect, otherwise, there is intersection, 
        // than iterate through the ones the intersect, and find the dups. 

    }
}
